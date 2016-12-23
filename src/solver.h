#ifndef __SOLVER_H
#define __SOLVER_H

#include "parser.h"
#include "clause.h"
#include "disjointset.h"
#include "opstack.h"
#include "statistic.h"
#include "lazytable.h"
#include "heap.h"
#include <sys/time.h>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <unordered_set>
using namespace std;


extern int solveDone;


class solver {

public:
    struct WatcherInfo {
        int clsid, wid;
        int prev=-1, next=-1;
        WatcherInfo() {};
        WatcherInfo(int clsid, int wid)
        :clsid(clsid), wid(wid) {}
    };

    typedef pair<int,int> pii;
    enum {
        INFINITE = 1023456789,
        HEURISTIC_NO_INIT = 1,
        HEURISTIC_MOM_INIT = 2,
        HEURISTIC_VSIDS = 4,
        RESTART_RUBY = 8,
        LEARN_UNSAT = 10,
        LEARN_ASSIGNMENT = 11,
        LEARN_CLAUSE = 12
    };
    static const int clauseSzThreshold = 10;
    Statistic statistic;

    // Init via CNF file and seperate independent subproblems
    void init(const char *filename);
    bool solve(int mode);
    inline vector<int> result();
    inline void printCNF();
    inline int size();


protected:
    int unsatAfterInit = 0;
    int sat = 1;
    int heuristicMode = HEURISTIC_VSIDS;

    int maxVarIndex;
    int oriClsNum;
    vector<Clause> clauses;
    vector<int> learntUnit;
    opStack var;
    int nowLevel = 0;

    // Each subproblem a solver
    vector<solver> subproblem;
    vector< vector<int> > mappingVar;
    void _init(const vector< vector<int> > &rth, int maxIdx);

    // Helper function for DPLL
    bool set(int var, bool val, int src=-1);
    void backtrack(int lv);
    int learnFromConflict(int &vid, int &sign, int &src);
    bool restart();

    bool _solve();
    int conflictingClsID = -1;
    Lazytable litMarker;
    inline int _resolve(int clsid, int x, vector<int> &prev);


    // Clause helper function
    inline bool evalClauesLit(int clsid, int id) const;
    inline bool evalClauesLit(const Clause &cls, int id) const;
    inline bool evalClauesWatchedLit(const WatcherInfo &info) const;
    inline bool evalClauesWatchedLit(int clsid, int wid) const;
    inline bool evalClauesWatchedLit(const Clause &cls, int wid) const;
    inline int updateClauseWatcher(const WatcherInfo &info);
    inline int updateClauseWatcher(int clsid, int wid);
    inline int updateClauseWatcher(Clause &cls, int wid);


    // 2 Literal Watching
    vector<WatcherInfo> watchers;
    vector<int> pos;
    vector<int> neg;

    // 2 Literal Watching helper function
    inline int getLit(const WatcherInfo &info) const;
    inline int getVar(const WatcherInfo &info) const;
    inline int getSign(const WatcherInfo &info) const;
    inline int getVal(const WatcherInfo &info) const;
    inline bool eval(const WatcherInfo &info) const;

    // Conflict Clause Learning Heuristic
    vector<int> firstUIP();
    void minimizeLearntCls(vector<int> &learnt);
    bool isFromUIP(int vid, int sign);

    // Branching Heuristic
    VarHeap varPriQueue;

    void heuristicInit_no();
    void heuristicInit_MOM();
    void heuristicInit_VSIDS();
    pii pickUnassignedVar();

};


// Return result
inline vector<int> solver::result() {
    if( !sat )
        return vector<int>(1, 0);
    vector<int> ret(maxVarIndex+1, 1);
    for(int i=1; i<=maxVarIndex; ++i)
        ret[i] = var.getVal(i) ? i : -i;
    return ret;
}

inline void solver::printCNF() {
    for(auto &cls : clauses) {
        for(int i=0; i<cls.size(); ++i)
            printf("%d ", cls.getLit(i));
        printf("0\n");
    }
}

inline int solver::size() {
    return maxVarIndex;
}


// Resolve helper
inline int solver::_resolve(int clsid, int x, vector<int> &prev) {

    int ret = 0;
    for(int i=0; i<clauses[clsid].size(); ++i) {
        int lit = clauses[clsid].getLit(i);
        int vid = clauses[clsid].getVar(i);
        int sign = clauses[clsid].getSign(i);
        if( vid == x || litMarker.get(vid) == sign ) continue;
        if( litMarker.get(vid) != -1 ) return -1;
        litMarker.set(vid, sign);
        if( var.getLv(vid) == nowLevel )
            ++ret;
        else
            prev.emplace_back(lit);
    }

    return ret;

}


// Clause helper function
inline bool solver::evalClauesLit(int clsid, int id) const {
    return evalClauesLit(clauses[clsid], id);
}
inline bool solver::evalClauesLit(const Clause &cls, int id) const {
    return var.getVal(cls.getVar(id)) == cls.getSign(id);
}

inline bool solver::evalClauesWatchedLit(const WatcherInfo &info) const {
    return evalClauesWatchedLit(info.clsid, info.wid);
}
inline bool solver::evalClauesWatchedLit(int clsid, int wid) const {
    return evalClauesWatchedLit(clauses[clsid], wid);
}
inline bool solver::evalClauesWatchedLit(const Clause &cls, int wid) const {
    return evalClauesLit(cls, cls.watcher[wid]);
}

inline int solver::updateClauseWatcher(const WatcherInfo &info) {
    return updateClauseWatcher(info.clsid, info.wid);
}
inline int solver::updateClauseWatcher(int clsid, int wid) {
    return updateClauseWatcher(clauses[clsid], wid);
}
inline int solver::updateClauseWatcher(Clause &cls, int wid) {

    for(int counter=cls.size(); counter; --counter) {

        cls.watchNext(wid);
        if( !cls.watchSame() &&
                (var.getVal(cls.getWatchVar(wid)) == 2 || evalClauesWatchedLit(cls, wid)) )
            return cls.getWatchLit(wid);

    }
    return cls.getWatchLit(wid);

}


// 2 Literal Watching helper function
inline int solver::getLit(const WatcherInfo &info) const {
    return clauses[info.clsid].getWatchLit(info.wid);
}
inline int solver::getVar(const WatcherInfo &info) const {
    return clauses[info.clsid].getWatchVar(info.wid);
}
inline int solver::getSign(const WatcherInfo &info) const {
    return clauses[info.clsid].getWatchSign(info.wid);
}
inline int solver::getVal(const WatcherInfo &info) const {
    return var.getVal(getVar(info));
}
inline bool solver::eval(const WatcherInfo &info) const {
    return evalClauesWatchedLit(clauses[info.clsid], info.wid);
}


#endif
