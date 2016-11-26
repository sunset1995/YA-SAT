#ifndef __CLAUSE_H
#define __CLAUSE_H

#include <cstdlib>
#include <vector>
using namespace std;


struct Clause {

    // Literal in clause. ex: -1 2 3 -4 5
    // watcher -> index of watched variable in lit
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

#endif
