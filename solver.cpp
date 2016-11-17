#include "solver.h"


void solver::init(const char *filename) {

    vector< vector<int> > tmp;
    parse_DIMACS_CNF(tmp, maxVarIndex, filename);

    var = opStack(maxVarIndex + 4);
    pos = vector< vector<WatcherInfo> >(maxVarIndex+4);
    neg = vector< vector<WatcherInfo> >(maxVarIndex+4);

    vector<int> unit;
    for(auto &cls : tmp) {
        if( cls.empty() ) unsatAfterInit = 1;
        else if( cls.size() == 1 ) unit.emplace_back(cls[0]);
        else {
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

bool solver::solve() {
    statistic.init();
    if( unsatAfterInit ) return false;
    sat = solve(1);
    statistic.stopTimer();
    return sat;
}
bool solver::solve(int nowAt) {
    ++nowLevel;
    statistic.maxDepth = max(nowLevel, statistic.maxDepth);
    while( nowAt <= maxVarIndex && var.getVal(nowAt)!=2 )
        ++nowAt;
    if( nowAt > maxVarIndex ) {
        --nowLevel;
        return true;
    }

    if( !set(nowAt, true) || !solve(nowAt+1) )
        var.backToLevel(nowLevel-1);
    else {
        --nowLevel;
        return true;
    }

    if( !set(nowAt, false) || !solve(nowAt+1) )
        var.backToLevel(nowLevel-1);
    else {
        --nowLevel;
        return true;
    }

    ++statistic.backtrackNum;
    --nowLevel;
    return false;
}
