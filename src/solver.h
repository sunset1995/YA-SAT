#ifndef __SOLVER_H
#define __SOLVER_H

#include "parser.h"
#include "clause.h"
#include "disjointset.h"
#include "opstack.h"
#include "statistic.h"
#include <sys/time.h>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <unordered_set>
using namespace std;


class solver {
public:
    typedef pair<int,int> pii;
    enum {
        INFINITE = 1023456789,
        HEURISTIC_NO = 1011,
        HEURISTIC_MOM = 1012,
    };
    struct WatcherInfo {
        int clsid, wid;
        WatcherInfo() {};
        WatcherInfo(int clsid, int wid)
        :clsid(clsid), wid(wid) {}
    };
    Statistic statistic;
    int unsatAfterInit = 0;
    int sat = 0;

    int maxVarIndex;
    vector<Clause> clauses;
    opStack var;
    DisjointSet dset;
    int nowSetID = 0;


    // Recursive level
    int nowLevel = 0;


    void init(const char *filename);

    bool evalClauesLit(int clsid, int id) const;
    bool evalClauesLit(const Clause &cls, int id) const;

    bool evalClauesWatchedLit(const WatcherInfo &info) const;
    bool evalClauesWatchedLit(int clsid, int wid) const;
    bool evalClauesWatchedLit(const Clause &cls, int wid) const;

    int updateClauseWatcher(const WatcherInfo &info);
    int updateClauseWatcher(int clsid, int wid);
    int updateClauseWatcher(Clause &cls, int wid);

    bool set(int var, bool val);
    void backToLevel(int lv);

    bool solve(int mode);
    bool _solve();


    // 2 Literal Watching
    vector< vector<WatcherInfo> > pos;
    vector< vector<WatcherInfo> > neg;
    inline int getLit(const WatcherInfo &info) const {
        return clauses[info.clsid].getWatchLit(info.wid);
    }
    inline int getVar(const WatcherInfo &info) const {
        return clauses[info.clsid].getWatchVar(info.wid);
    }
    inline int getSign(const WatcherInfo &info) const {
        return clauses[info.clsid].getWatchSign(info.wid);
    }
    inline int getVal(const WatcherInfo &info) const {
        return var.getVal(getVar(info));
    }
    inline bool eval(const WatcherInfo &info) const {
        return evalClauesWatchedLit(clauses[info.clsid], info.wid);
    }

    // Branching Heuristic
    int staticOrderFrom;
    vector<pii> staticOrder;
    pii (solver::*pickUnassignedVar)() = NULL;

    void heuristicInit_no();
    void heuristicInit_MOM();
    pii heuristic_static();
};

#endif
