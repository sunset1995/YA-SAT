#include "solver.h"


// Solver helper function
static bool satisfyAlready(const vector<int> &cls) {
    unordered_set<int> checker;
    for(auto &v : cls) {
        if( checker.find(-v) != checker.end() )
            return true;
        checker.insert(v);
    }
    return false;
}


// Init solver with cnf file
void solver::init(const char *filename) {

    // Init with empty solver
    *this = solver();

    // Get raw clause from cnf file
    vector< vector<int> > raw;
    parse_DIMACS_CNF(raw, maxVarIndex, filename);

    // Init assignment stack
    var = opStack(maxVarIndex+4);

    // Init database with all clause which has 2 or more literal in raw database
    // Eliminate all unit clause and check whether there is empty clause
    vector<int> unit;
    for(auto &cls : raw) {
        if( cls.empty() ) unsatAfterInit = 1;
        else if( cls.size() == 1 ) unit.emplace_back(cls[0]);
        else if( !satisfyAlready(cls) ) {
            clauses.push_back(Clause());
            clauses.back().watcher[0] = 0;
            clauses.back().watcher[1] = (cls.size() >> 1);
            clauses.back().lit = move(cls);
        }
    }

    // Init two watching check list
    pos = vector< vector<WatcherInfo> >(maxVarIndex+4);
    neg = vector< vector<WatcherInfo> >(maxVarIndex+4);
    int cid = 0;
    for(auto &cls : clauses) {
        int id = cls.getWatchVar(0);
        if( cls.getWatchSign(0) )
            pos[id].emplace_back(WatcherInfo(cid, 0));
        else
            neg[id].emplace_back(WatcherInfo(cid, 0));

        id = cls.getWatchVar(1);
        if( cls.getWatchSign(1) )
            pos[id].emplace_back(WatcherInfo(cid, 1));
        else
            neg[id].emplace_back(WatcherInfo(cid, 1));
        ++cid;
    }

    // Assign and run BCP for all unit clause
    for(auto lit : unit)
        unsatAfterInit |= !set(abs(lit), lit>0);

    // Identify independent subproblem via disjoint set
    dset.init(maxVarIndex+4);
    for(auto &cls : clauses) {
        int id = 0;
        while( id<cls.size() && var.getVal(cls.getVar(id))!=2 )
            ++id;
        for(int i=id+1; i<cls.size(); ++i)
            if( var.getVal(cls.getVar(i))==2 )
                dset.unionSet(cls.getVar(i), cls.getVar(id));
    }

}


// Assign id=val@nowLevel and run BCP recursively
bool solver::set(int id, bool val, int src) {

    // If id is already set, check consistency
    if( var.getVal(id) != 2 ) {
        conflictingClsID = -1;
        return var.getVal(id) == val;
    }

    // Set id=val@nowLevel
    var.set(id, val, nowLevel, src);

    // Update 2 literal watching
    bool ret = true;
    vector<WatcherInfo> &lst = (val ? neg[id] : pos[id]);
    vector<int> del(lst.size(), false);
    for(int i=0; i<lst.size(); ++i) {

        // Update watcher
        updateClauseWatcher(lst[i]);
        if( getVal(lst[i]) == 2 || eval(lst[i]) ) {
            // Watcher reaches an pending/satisfied variable

            // Push this watcher to corresponding check list
            if( getSign(lst[i]) )
                pos[getVar(lst[i])].emplace_back(lst[i]);
            else
                neg[getVar(lst[i])].emplace_back(lst[i]);

            // Delete this watcher from current check list
            del[i] = true;

        }
        else {
            // Watcher run through all clause back to original one
            // Can't find next literal to watch

            // b = alternative watcher in this clause
            WatcherInfo b(lst[i].clsid, lst[i].wid^1);
            if( getVal(b) == 2 ) {
                if( !set(getVar(b), getSign(b), lst[i].clsid) ) {
                    ret = false;
                    break;
                }
            }
            else if( !eval(b) ) {
                conflictingClsID = lst[i].clsid;
                ret =  false;
                break;
            }

        }

    }

    int i = 0, j = -1;
    while( i<lst.size() && !del[i] )
        ++i;
    j = i-1;
    for(; i<lst.size(); ++i)
        if( !del[i] )
            lst[++j] = lst[i];
    lst.resize(j+1);

    // BCP done successfully without conflict
    return ret;

}


bool solver::solve(int mode) {

    // Init statistic and start timer
    statistic.init();
    if( unsatAfterInit ) return sat = false;

    // Init DPLL
    litMarker.init(maxVarIndex+4);
    sat = true;
    nowLevel = 0;

    // Solve each independent subproblems
    for(int i=1; i<=maxVarIndex && sat; ++i) {

        if( dset.findRoot(i) != i ) continue;

        nowSetID = i;

        // Init for specific heuristic
        // This must be done before each subproblems
        if( mode == HEURISTIC_NO ) {
            heuristicInit_no();
            pickUnassignedVar = &solver::heuristic_static;
        }
        else if( mode == HEURISTIC_MOM ) {
            heuristicInit_MOM();
            pickUnassignedVar = &solver::heuristic_static;
        }
        else {
            fprintf(stderr, "Unknown solver mode\n");
            exit(1);
        }

        sat &= _solve();

    }
    statistic.stopTimer();

    return sat;

}

bool solver::_solve() {

    // This subproblem startting stack index
    const int bound = nowLevel;

    // Main loop for DPLL
    while( true ) {

        ++nowLevel;
        statistic.maxDepth = max(statistic.maxDepth, nowLevel);
        staticOrderFrom = 0;
        pii decision = (this->*pickUnassignedVar)();
        if( decision.first == -1 )
            return true;
        int vid = decision.first;
        int sign = decision.second;

        while( !set(vid, sign) ) {

            if( conflictingClsID == -1 )
                return false;

            vector<int> learnt = firstUIP();
            if( learnt.empty() )
                return false;

            // Determined cronological backtracking
            int backlv = bound;
            int towatch = -1;
            for(int i=learnt.size()-2; i>=0; --i)
                if( var.getLv(abs(learnt[i])) > backlv ) {
                    backlv = var.getLv(abs(learnt[i]));
                    towatch = i;
                }

            // Learn one assignment
            if( learnt.size() == 1 || backlv == bound ) {
                ++statistic.backtrackNum;
                ++statistic.learnAssignment;
                var.backToLevel(bound);
                statistic.maxJumpBack = max(statistic.maxJumpBack, nowLevel-bound);
                nowLevel = bound;
                int uip = learnt.back();
                if( !set(abs(uip), uip>0) )
                    return false;
                break;
            }

            // Add conflict clause
            statistic.maxLearntSz = max(statistic.maxLearntSz, int(learnt.size()));
            statistic.totalLearntSz += learnt.size();
            clauses.push_back(Clause());
            clauses.back().watcher[0] = towatch;
            clauses.back().watcher[1] = learnt.size() - 1;
            clauses.back().lit = move(learnt);

            int cid = clauses.size() - 1;
            int id = clauses.back().getWatchVar(0);
            if( clauses.back().getWatchSign(0) )
                pos[id].emplace_back(WatcherInfo(cid, 0));
            else
                neg[id].emplace_back(WatcherInfo(cid, 0));

            id = clauses.back().getWatchVar(1);
            if( clauses.back().getWatchSign(1) )
                pos[id].emplace_back(WatcherInfo(cid, 1));
            else
                neg[id].emplace_back(WatcherInfo(cid, 1));

            ++statistic.backtrackNum;
            ++statistic.learnCls;
            statistic.maxJumpBack = max(statistic.maxJumpBack, nowLevel-backlv);
            var.backToLevel(backlv-1);
            nowLevel = backlv;
            vid = var.topNext().var;
            sign = var.topNext().val;

        }

    }

    return false;
}


/******************************************************
    Implementing Conflict Clause Learning Heuristic
******************************************************/
vector<int> solver::firstUIP() {

    // Init
    litMarker.clear();
    vector<int> learnt;
    int todoNum = _resolve(conflictingClsID, -1, learnt);
    if( todoNum == -1 )
        return vector<int>();

    // Resolve and find 1UIP
    int top = var._top;
    while( todoNum > 1 ) {
        while( litMarker.get(var.stk[top].var) == -1 )
            --top;
        int nowNum = _resolve(var.stk[top].src, var.stk[top].var, learnt);
        if( nowNum == -1 )
            return vector<int>();
        todoNum += nowNum - 1;
        --top;
    }

    // Put 1UIP at back of the vector
    while( litMarker.get(var.stk[top].var) == -1 )
        --top;
    int uip = (var.stk[top].val > 0 ? -var.stk[top].var : var.stk[top].var);
    learnt.emplace_back(uip);

    // Minimization
    minimizeLearntCls(learnt);

    return learnt;

}

void solver::minimizeLearntCls(vector<int> &learnt) {

    // Minimize clause
    litMarker.clear();
    for(int i=0; i<learnt.size(); ++i)
        litMarker.set(abs(learnt[i]), learnt[i]>0);
    vector<int> eliminateMark(learnt.size(), false);
    int eliminateNum = 0;

    // Check all literals in learnt clause except 1UIP
    for(int i=learnt.size()-2; i>=0; --i) {
        int src = var.getSrc(abs(learnt[i]));
        if( src == -1 )
            continue;

        bool selfSubsumed = true;
        for(int j=0; j<clauses[src].size(); ++j) {
            int vid = clauses[src].getVar(j);
            int sign = clauses[src].getSign(j)>0;
            if( abs(learnt[i])!=vid && !isFromUIP(vid, sign) ) {
                selfSubsumed = false;
                break;
            }
        }
        if( selfSubsumed ) {
            ++eliminateNum;
            eliminateMark[i] = true;
        }
    }
    if( eliminateNum ) {
        int j = -1;
        for(int i=0; i<learnt.size(); ++i)
            if( eliminateMark[i] == false )
                learnt[++j] = learnt[i];
        learnt.resize(j+1);
    }

}

bool solver::isFromUIP(int vid, int sign) {

    if( litMarker.get(vid) != -1 )
        return litMarker.get(vid) == sign;

    int src = var.getSrc(vid);
    if( src == -1 ) {
        litMarker.set(vid, 2);
        return false;
    }

    for(int i=0; i<clauses[src].size(); ++i) {
        int nv = clauses[src].getVar(i);
        int ns = clauses[src].getSign(i);
        if( nv!=vid && isFromUIP(nv, ns) == false ) {
            litMarker.set(vid, 2);
            return false;
        }
    }
    litMarker.set(vid, sign);
    return true;

}


/******************************************************
    Implementing Branching Heuristic
******************************************************/
void solver::heuristicInit_no() {
    staticOrderFrom = 0;
    staticOrder.resize(maxVarIndex);
    for(int i=1; i<=maxVarIndex; ++i)
        staticOrder[i-1] = {i, 1};
}

void solver::heuristicInit_MOM() {
    staticOrderFrom = 0;
    staticOrder.resize(maxVarIndex);
    for(int i=1; i<=maxVarIndex; ++i)
        staticOrder[i-1] = {i, 1};

    vector<long long> scorePos(maxVarIndex + 4, 0);
    vector<long long> scoreNeg(maxVarIndex + 4, 0);
    vector<long long> score(maxVarIndex + 4, 0);
    for(auto &cls : clauses) {
        if( cls.size() > 3 )
            continue;
        for(int i=0; i<cls.size(); ++i)
            if( cls.getSign(i) )
                ++scorePos[cls.getVar(i)];
            else
                ++scoreNeg[cls.getVar(i)];
    }
    for(int i=1; i<=maxVarIndex; ++i) {
        if( dset.sameSet(i, nowSetID) )
            score[i] = scorePos[i] + scoreNeg[i];
        else
            score[i] = -1;
    }
    sort(staticOrder.begin(), staticOrder.end(), [&score](const pii &l, const pii &r) {
        return score[l.first] > score[r.first];
    });
    for(auto &v : staticOrder)
        if( scoreNeg[v.first] > scorePos[v.first] )
            v.second = 0;
}

pair<int,int> solver::heuristic_static() {
    for(int i=staticOrderFrom; i<staticOrder.size(); ++i)
        if( var.getVal(staticOrder[i].first)==2 &&
                dset.sameSet(staticOrder[i].first, nowSetID) ) {
            staticOrderFrom = i+1;
            return staticOrder[i];
        }
    return {-1, 0};
}
