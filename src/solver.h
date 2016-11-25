#ifndef __SOLVER_H
#define __SOLVER_H

#include "parser.h"
#include <sys/time.h>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <unordered_set>
using namespace std;


struct Statistic {
    int backtrackNum = 0;
    int maxDepth = 0;
    struct timeval start, end;
    inline void init() {
        backtrackNum = maxDepth = 0;
        gettimeofday(&start, 0);
        gettimeofday(&end, 0);
    }
    inline void stopTimer() {
        gettimeofday(&end, 0);
    }
    inline double elapseTime() {
        int sec = end.tv_sec - start.tv_sec;
        int usec = end.tv_usec - start.tv_usec;
        return sec + (usec/1000000.0);
    }
};


class opStack {
public:
    struct op {
        int var, val;
        int trie = -1, pickerInfo = 0;
    };
    vector<int> idx;
    vector<int> level;
    vector<op> stk;
    int top = -1;
    opStack() {}
    opStack(int n):idx(n), level(n), stk(n) {
        level[0] = -1;
    }
    inline bool notSet(int id) const {
        return idx[id] > top || stk[idx[id]].var != id;
    }
    inline int getVal(int id) const {
        if( notSet(id) ) return 2;
        return stk[idx[id]].val;
    }
    inline void set(int id, int val, int lv) {
        //fprintf(stderr, "set %d = %d@%d\n", id, val, lv);
        ++top;
        stk[top].var = id;
        stk[top].val = val;
        level[lv] = top;
        idx[id] = top;
    }
    inline void backToLevel(int lv) {
        //fprintf(stderr, "backToLevel %d\n", lv);
        top = level[lv];
    }
};


class Clause {
public:
    vector<int> lit;
    int watcher[2];
    inline int size() const {
        return lit.size();
    }
    inline int getLit(int id) const {
        return lit[id];
    }
    inline int getVar(int id) const {
        return abs(lit[id]);
    }
    inline int getSign(int id) const {
        return lit[id] > 0;
    }
    inline int getWatchLit(int wid) const {
        return getLit(watcher[wid]);
    }
    inline int getWatchVar(int wid) const {
        return getVar(watcher[wid]);
    }
    inline int getWatchSign(int wid) const {
        return getSign(watcher[wid]);
    }
    inline bool watchSame() const {
        return watcher[0] == watcher[1];
    }
    inline void watchNext(int wid) {
        int v = watcher[wid] + 1;
        watcher[wid] = v & (int(v == lit.size()) - 1);
    }
};


class DisjointSet {
public:
    vector<int> root;
    vector<int> sz;
    inline void init(int n) {
        root.resize(n);
        sz.resize(n, 1);
        for(int i=0; i<n; ++i)
            root[i] = i;
    }
    int findRoot(int id) {
        return id==root[id] ? id : (root[id] = findRoot(root[id]));
    }
    inline bool sameSet(int a, int b) {
        return findRoot(a) == findRoot(b);
    }
    inline void unionSet(int a, int b) {
        a = findRoot(a);
        b = findRoot(b);
        if( a!=b ) {
            root[b] = a;
            sz[a] += sz[b];
        }
    }
};


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
