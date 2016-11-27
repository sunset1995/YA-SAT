#include "solver.h"


bool satisfyAlready(const vector<int> &cls) {
    unordered_set<int> checker;
    for(auto &v : cls) {
        if( checker.find(-v) != checker.end() )
            return true;
        checker.insert(v);
    }
    return false;
}


void solver::init(const char *filename) {

    *this = solver();

    vector< vector<int> > tmp;
    parse_DIMACS_CNF(tmp, maxVarIndex, filename);

    var = opStack(maxVarIndex + 4);
    pos = vector< vector<WatcherInfo> >(maxVarIndex+4);
    neg = vector< vector<WatcherInfo> >(maxVarIndex+4);

    vector<int> unit;
    for(auto &cls : tmp) {
        if( cls.empty() ) unsatAfterInit = 1;
        else if( cls.size() == 1 ) unit.emplace_back(cls[0]);
        else if( !satisfyAlready(cls) ) {
            clauses.push_back(Clause());
            clauses.back().watcher[0] = 0;
            clauses.back().watcher[1] = (cls.size() >> 1);
            clauses.back().lit = move(cls);
        }
    }

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

    for(auto lit : unit)
        unsatAfterInit |= !set(abs(lit), lit>0);

    dset.init(maxVarIndex+4);
    for(auto &cls : clauses) {
        int id = 0;
        while( id<cls.size() && var.getVal(cls.getVar(id))!=2 )
            ++id;
        if( id == cls.size() ) continue;
        for(int i=id+1; i<cls.size(); ++i)
            dset.unionSet(cls.getVar(id), cls.getVar(i));
    }

}


bool solver::evalClauesLit(int clsid, int id) const {
    return evalClauesLit(clauses[clsid], id);
}
bool solver::evalClauesLit(const Clause &cls, int id) const {
    return var.getVal(cls.getVar(id)) == cls.getSign(id);
}


bool solver::evalClauesWatchedLit(const WatcherInfo &info) const {
    return evalClauesWatchedLit(info.clsid, info.wid);
}
bool solver::evalClauesWatchedLit(int clsid, int wid) const {
    return evalClauesWatchedLit(clauses[clsid], wid);
}
bool solver::evalClauesWatchedLit(const Clause &cls, int wid) const {
    return evalClauesLit(cls, cls.watcher[wid]);
}


int solver::updateClauseWatcher(const WatcherInfo &info) {
    return updateClauseWatcher(info.clsid, info.wid);
}
int solver::updateClauseWatcher(int clsid, int wid) {
    return updateClauseWatcher(clauses[clsid], wid);
}
int solver::updateClauseWatcher(Clause &cls, int wid) {

    for(int counter=cls.size(); counter; --counter) {
        
        cls.watchNext(wid);
        if( !cls.watchSame() && 
                (var.getVal(cls.getWatchVar(wid)) == 2 || evalClauesWatchedLit(cls, wid)) )
            return cls.getWatchLit(wid);

    }
    return cls.getWatchLit(wid);

}


bool solver::set(int id, bool val) {   

    if( var.getVal(id) != 2 )
        return var.getVal(id) == val;
    var.set(id, val, nowLevel);
    vector<WatcherInfo> &lst = (val ? neg[id] : pos[id]);
    for(int i=lst.size()-1; i>=0; --i) {

        updateClauseWatcher(lst[i]);
        if( getVal(lst[i]) == 2 || eval(lst[i]) ) {
            if( getSign(lst[i]) )
                pos[getVar(lst[i])].emplace_back(lst[i]);
            else
                neg[getVar(lst[i])].emplace_back(lst[i]);
            swap(lst[i], lst.back());
            lst.pop_back();
        }
        else {
            WatcherInfo b(lst[i].clsid, lst[i].wid^1);
            if( getVal(b) == 2 ) {
                if( !set(getVar(b), getSign(b)) )
                    return false;
            }
            else if( !eval(b) )
                return false;
        }

    }
    return true;

}

void solver::backToLevel(int lv) {
    var.backToLevel(lv);
}


bool solver::solve(int mode) {
    statistic.init();
    if( unsatAfterInit ) return false;

    sat = true;
    for(int i=1; i<=maxVarIndex && sat; ++i) {

        if( dset.findRoot(i) != i ) continue;
        
        nowSetID = i;

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

        nowLevel = 0;
        sat &= _solve();

    }
    statistic.stopTimer();
    return sat;
}
bool solver::_solve() {

    while( true ) {

        opStack::op &now = var.stk[var.top+1];

        if( now.trie == 2 ) {

            // Backtracking
            now.trie = -1;
            staticOrderFrom = now.pickerInfo;
            ++statistic.backtrackNum;
            if( --nowLevel == 0 )
                break;
            backToLevel(nowLevel-1);
            continue;

        }

        if( now.trie == -1 ) {

            // Branching
            ++nowLevel;
            statistic.maxDepth = max(nowLevel, statistic.maxDepth);

            now.pickerInfo = staticOrderFrom;
            pii decision = (this->*pickUnassignedVar)();
            now.var = decision.first;
            now.val = decision.second;
            now.trie = 0;

            if( now.var == -1 ) return true;

        }

        if( !set(now.var, now.val ^ (now.trie++)) )
            backToLevel(nowLevel-1);
        else
            continue;

    }

    return false;
}


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
