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
    vector<WatcherInfo> &lst = (val ? neg[id] : pos[id]);
    for(int i=lst.size()-1; i>=0; --i) {

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
            swap(lst[i], lst.back());
            lst.pop_back();

        }
        else {
            // Watcher run through all clause back to original one
            // Can't find next literal to watch

            // b = alternative watcher in this clause
            WatcherInfo b(lst[i].clsid, lst[i].wid^1);
            if( getVal(b) == 2 ) {
                if( !set(getVar(b), getSign(b), lst[i].clsid) )
                    return false;
            }
            else if( !eval(b) ) {
                conflictingClsID = lst[i].clsid;
                return false;
            }

        }

    }

    // BCP done successfully without conflict
    return true;

}


bool solver::solve(int mode) {

    // Init statistic and start timer
    statistic.init();
    if( unsatAfterInit ) return false;

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

            // Init
            litMarker.clear();
            vector<int> curr;
            vector<int> prev;
            if( !_resolve(conflictingClsID, -1, curr, prev) )
                return false;

            // Resolve and find 1UIP
            int uip = curr.back();
            int notFoundUIP = 1;
            for(int i=0; i<curr.size()-notFoundUIP; ++i) {
                int vid = abs(curr[i]);
                int clsid = var.getSrc(vid);
                if( clsid != -1 ) {
                    if( !_resolve(clsid, vid, curr, prev) )
                        return false;
                }
                else {
                    uip = curr[i];
                    notFoundUIP = 0;
                }
            }
            if( notFoundUIP )
                uip = curr.back();

            // Determined cronological backtracking
            int backlv = bound;
            int towatch = -1;
            for(int i=0; i<prev.size(); ++i)
                if( var.getLv(abs(prev[i])) > backlv ) {
                    backlv = var.getLv(abs(prev[i]));
                    towatch = i;
                }

            // Learn one assignment
            if( prev.empty() || backlv == bound ) {
                ++statistic.backtrackNum;
                ++statistic.learnAssignment;
                var.backToLevel(bound);
                nowLevel = bound;
                statistic.maxJumpBack = max(statistic.maxJumpBack, nowLevel-bound);
                if( !set(abs(uip), uip>0) )
                    return false;
                break;
            }

            // Add conflict clause
            prev.emplace_back(uip);
            clauses.push_back(Clause());
            clauses.back().watcher[0] = towatch;
            clauses.back().watcher[1] = prev.size() - 1;
            clauses.back().lit = move(prev);

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


/****************************************
    Implementing Branching Heuristic
****************************************/
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
