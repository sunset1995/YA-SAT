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

static void appendListWatcher(vector<solver::WatcherInfo> &pool, int &head, int eleid) {
    // Empty case
    if( head == -1 ) {
        head = eleid;
        pool[eleid].prev = pool[eleid].next = eleid;
        return;
    }
    int prev = pool[head].prev;
    pool[eleid].next = head;
    pool[eleid].prev = prev;
    pool[prev].next = eleid;
    pool[head].prev = eleid;
}

static void swapListWatcher(vector<solver::WatcherInfo> &pool, int &from, int &to, int eleid) {
    // Check list head
    if( eleid == from ) {
        if( pool[from].next == from )
            from = -1;
        else
            from = pool[eleid].next;
    }
    // Remove from current list
    pool[pool[eleid].prev].next = pool[eleid].next;
    pool[pool[eleid].next].prev = pool[eleid].prev;
    // Add to target list
    appendListWatcher(pool, to, eleid);
}


// Init solver with cnf file
void solver::init(const char *filename) {

    // Init with empty solver
    *this = solver();

    // Get raw clause from cnf file
    vector< vector<int> > raw;
    parse_DIMACS_CNF(raw, maxVarIndex, filename);
    var = opStack(maxVarIndex+4);

    // Identify independent subproblem via disjoint set
    DisjointSet dset;
    dset.init(maxVarIndex+4);
    for(auto &cls : raw) 
        for(int i=1,rid=abs(cls[0]); i<cls.size(); ++i)
            dset.unionSet(rid, abs(cls[i]));

    // Find all root of disjointset
    int nInd = 0;
    vector<int> roots;
    for(int i=1; i<=maxVarIndex; ++i)
        if( dset.findRoot(i) == i ) {
            ++nInd;
            roots.emplace_back(i);
        }

    // If all variables are related, treat as independent subproblem
    if( nInd <= 1 ) {
        _init(raw, maxVarIndex);
        return;
    }

    // Sort via # of variables
    sort(roots.begin(), roots.end(), [&dset](const int l, const int r) {
        return dset.setSz(l) < dset.setSz(r);
    });

    // Mapping variables to each subproblem's id
    vector<int> pid(maxVarIndex+4);
    for(int i=0; i<nInd; ++i)
        pid[roots[i]] = i;
    for(int i=1; i<=maxVarIndex; ++i)
        pid[i] = pid[dset.findRoot(i)];

    // Mapping between global variables and local variables
    mappingVar.resize(nInd);
    for(int i=0; i<nInd; ++i) {
        mappingVar[i].resize(dset.setSz(roots[i])+1, -1);
    }
    vector<int> num(nInd, 0);
    vector<int> backmp(maxVarIndex+4);
    for(int i=1; i<=maxVarIndex; ++i) {
        ++num[pid[i]];
        mappingVar[ pid[i] ][ num[pid[i]] ] = i;
        backmp[i] = num[pid[i]];
    }

    // Split cnf into independent subsets
    vector< vector< vector<int> > > cls(nInd);
    for(int i=0; i<raw.size(); ++i)
        cls[pid[abs(raw[i][0])]].emplace_back(move(raw[i]));

    // Init each subproblems
    subproblem.resize(nInd);
    for(int i=0; i<nInd && !unsatAfterInit; ++i) {
        if( cls[i].empty() )
            continue;
        for(auto &c : cls[i]) {
            for(auto &v : c)
                v = (v > 0 ? backmp[abs(v)] : -backmp[abs(v)]);
        }
        subproblem[i]._init(move(cls[i]), dset.setSz(roots[i]));
        unsatAfterInit |= subproblem[i].unsatAfterInit;
    }

}

void solver::_init(const vector< vector<int> > &rth, int maxIdx) {

    // Init with empty solver
    *this = solver();

    // Init assignment stack
    maxVarIndex = maxIdx;
    var = opStack(maxVarIndex+4);

    // Init database with all clause which has 2 or more literal in raw database
    // Eliminate all unit clause and check whether there is empty clause
    vector<int> unit;
    for(auto &cls : rth) {
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
    watchers = vector<WatcherInfo>(clauses.size()*2);
    pos = vector<int>(maxVarIndex+4, -1);
    neg = vector<int>(maxVarIndex+4, -1);
    for(int cid=0, wid=-1; cid<clauses.size(); ++cid) {
        Clause &cls = clauses[cid];
        int id = cls.getWatchVar(0);
        watchers[++wid] = WatcherInfo(cid, 0);
        if( cls.getWatchSign(0) )
            appendListWatcher(watchers, pos[id], wid);
        else
            appendListWatcher(watchers, neg[id], wid);

        id = cls.getWatchVar(1);
        watchers[++wid] = WatcherInfo(cid, 1);
        if( cls.getWatchSign(1) )
            appendListWatcher(watchers, pos[id], wid);
        else
            appendListWatcher(watchers, neg[id], wid);
    }

    // Assign and run BCP for all unit clause
    for(auto lit : unit)
        unsatAfterInit |= !set(abs(lit), lit>0);

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
    int &head = (val ? neg[id] : pos[id]);
    int idx = head;
    while( idx != -1 ) {

        WatcherInfo &now = watchers[idx];
        int next = (now.next == head ? -1 : now.next);

        // Update watcher
        updateClauseWatcher(now);
        if( getVal(now) == 2 || eval(now) ) {
            // Watcher reaches an pending/satisfied variable

            // Push this watcher to corresponding check list
            if( getSign(now) )
                swapListWatcher(watchers, head, pos[getVar(now)], idx);
            else
                swapListWatcher(watchers, head, neg[getVar(now)], idx);

        }
        else {
            // Watcher run through all clause back to original one
            // Can't find next literal to watch

            // b = alternative watcher in this clause
            WatcherInfo b(now.clsid, now.wid^1);
            if( getVal(b) == 2 ) {
                if( !set(getVar(b), getSign(b), now.clsid) ) {
                    ret = false;
                    break;
                }
            }
            else if( !eval(b) ) {
                conflictingClsID = now.clsid;
                ret =  false;
                break;
            }

        }

        idx = next;

    }

    // BCP done successfully without conflict
    return ret;

}


void solver::backtrack(int lv) {
    for(int i=var._top; i>=0 && var.stk[i].lv>lv; --i)
        varPriQueue.restore(var.stk[i].var);
    var.backToLevel(lv);
}


// Conflicting.
// It will backtrack to the decision level
// where causing conflict clause become unit
// and set vid, sign, src indicating unit variable information.
int solver::learnFromConflict(int &vid, int &sign, int &src) {

    vector<int> learnt = firstUIP();
    if( learnt.empty() )
        return LEARN_UNSAT;

    // Determined cronological backtracking
    int backlv = 0;
    int towatch = -1;
    for(int i=learnt.size()-2; i>=0; --i)
        if( var.getLv(abs(learnt[i])) > backlv ) {
            backlv = var.getLv(abs(learnt[i]));
            towatch = i;
        }

    // Learn one assignment
    if( learnt.size() == 1 || backlv == 0 ) {
        ++statistic.backtrackNum;
        ++statistic.learnAssignment;
        backtrack(0);
        statistic.maxJumpBack = max(statistic.maxJumpBack, nowLevel);
        nowLevel = 0;
        int uip = learnt.back();
        if( !set(abs(uip), uip>0) )
            return LEARN_UNSAT;
        return LEARN_ASSIGNMENT;
    }

    // Add conflict clause
    statistic.maxLearntSz = max(statistic.maxLearntSz, int(learnt.size()));
    statistic.totalLearntSz += learnt.size();
    if( heuristicMode == HEURISTIC_VSIDS ) {
        if( statistic.learnCls % 10000 == 0 ) {
            varPriQueue.normMaxTo(1.0);
            varScore = 1.0;
        }
        varScore *= 1.05;
        for(auto &v : learnt)
            varPriQueue.increasePri(abs(v), varScore, v>0);
    }
    clauses.push_back(Clause());
    clauses.back().watcher[0] = towatch;           // Latest
    clauses.back().watcher[1] = learnt.size() - 1; // Learnt
    clauses.back().lit = move(learnt);

    int cid = clauses.size() - 1;
    int id = clauses.back().getWatchVar(0);
    watchers.emplace_back(WatcherInfo(cid, 0));
    if( clauses.back().getWatchSign(0) )
        appendListWatcher(watchers, pos[id], watchers.size()-1);
    else
        appendListWatcher(watchers, neg[id], watchers.size()-1);

    id = clauses.back().getWatchVar(1);
    watchers.emplace_back(WatcherInfo(cid, 1));
    if( clauses.back().getWatchSign(1) )
        appendListWatcher(watchers, pos[id], watchers.size()-1);
    else
        appendListWatcher(watchers, neg[id], watchers.size()-1);

    ++statistic.backtrackNum;
    ++statistic.learnCls;
    statistic.maxJumpBack = max(statistic.maxJumpBack, nowLevel-backlv);
    backtrack(backlv);
    nowLevel = backlv;
    vid = clauses.back().getWatchVar(1);
    sign = clauses.back().getWatchSign(1);
    src = clauses.size()-1;

    return LEARN_CLAUSE;

}


bool solver::solve(int mode) {

    heuristicMode = mode;

    // Init statistic and start timer
    statistic.init();
    if( unsatAfterInit ) return sat = false;

    if( subproblem.empty() ) {
        // This solver itself is an independent subproblem
        litMarker.init(maxVarIndex+4);
        nowLevel = 0;

        // Init for specific heuristic
        // This must be done before each subproblems
        if( mode == HEURISTIC_NO )
            heuristicInit_no();
        else if( mode == HEURISTIC_MOM )
            heuristicInit_MOM();
        else if( mode == HEURISTIC_VSIDS )
            heuristicInit_VSIDS();
        else {
            fprintf(stderr, "Unknown solver mode\n");
            exit(1);
        }
        varScore = 1.0;
        sat = _solve();
    }
    else {
        // Top level containing multiple subproblems
        for(int i=0; i<subproblem.size(); ++i) {

            if( subproblem[i].clauses.empty() )
                continue;

            subproblem[i].solve(mode);
            vector<int> result = subproblem[i].result();
            if( result[0] == 0 ) {
                subproblem[i].printCNF();
                sat = false;
                break;
            }
            const vector<int> &mp = mappingVar[i];
            for(int j=1; j<result.size(); ++j)
                var.set(mp[j], result[j]>0, 0, -1);
        }
    }

    statistic.stopTimer();
    return sat;

}

bool solver::_solve() {

    // Main loop for DPLL
    while( true ) {

        ++nowLevel;
        statistic.maxDepth = max(statistic.maxDepth, nowLevel);
        pii decision = pickUnassignedVar();
        if( decision.first == -1 )
            return true;
        int vid = decision.first;
        int sign = decision.second;
        int src = -1;

        while( !set(vid, sign, src) ) {

            if( conflictingClsID == -1 )
                return false;

            int learnResult = learnFromConflict(vid, sign, src);
            if( learnResult == LEARN_UNSAT )
                return false;
            else if( learnResult == LEARN_ASSIGNMENT )
                break;

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
    varPriQueue.init(maxVarIndex);
    for(int i=1; i<=maxVarIndex; ++i)
        varPriQueue.increaseInitPri(i, double(rand()) / RAND_MAX);
    varPriQueue.heapify();
}

void solver::heuristicInit_MOM() {
    varPriQueue.init(maxVarIndex);
    for(auto &cls : clauses)
        for(int i=0; i<cls.size(); ++i)
            varPriQueue.increaseInitPri(cls.getVar(i), 1.0);
    for(int i=1; i<=maxVarIndex; ++i)
        varPriQueue.increaseInitPri(i, double(rand()) / RAND_MAX);
    varPriQueue.heapify();
}

void solver::heuristicInit_VSIDS() {
    varPriQueue.init(maxVarIndex);
    for(auto &cls : clauses)
        for(int i=0; i<cls.size(); ++i)
            varPriQueue.increaseInitPri(cls.getVar(i), 1.0, cls.getSign(i));
    for(int i=1; i<=maxVarIndex; ++i)
        varPriQueue.increaseInitPri(i, double(rand()) / RAND_MAX);
    varPriQueue.heapify();
}

pair<int,int> solver::pickUnassignedVar() {
    // Find next decision
    while( true ) {
        if( varPriQueue.size() == 0 )
            return {-1, 0};
        int vid = abs(varPriQueue.top());
        int sign = varPriQueue.top()>0;
        varPriQueue.pop();
        if( var.getVal(vid)==2 )
            return {vid, sign};
    }
}
