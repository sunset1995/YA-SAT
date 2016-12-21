#ifndef __OPSTACK_H
#define __OPSTACK_H

#include <vector>
using namespace std;


class opStack {

public:

    struct op {
        int var, val, lv, src;
    };
    vector<op> stk;

    opStack() {}
    opStack(int n)
    :stk(n), idx(n), level(n) {
        level[0] = -1;
    }
    
    inline bool notSet(int id) const {
        return idx[id] > _top || stk[idx[id]].var != id;
    }
    inline int getVal(int id) const {
        return notSet(id) ? 2 : stk[idx[id]].val;
    }
    inline int getLv(int id) const {
        return notSet(id) ? -1 : stk[idx[id]].lv;
    }
    inline int getSrc(int id) const {
        return notSet(id) ? -1 : stk[idx[id]].src;
    }
    inline op& topNext() {
        // Return variable on decision or empty slot
        return stk[_top+1];
    }
    inline void set(int id, int val, int lv, int src) {
        // Set id = val@lv
        ++_top;
        stk[_top].var = id;
        stk[_top].val = val;
        stk[_top].lv = lv;
        stk[_top].src = src;
        idx[id] = _top;
        level[lv] = _top;
    }
    inline void backToLevel(int lv) {
        // Move top pointer to that level's last assignment
        _top = level[lv];
    }

    vector<int> idx;
    vector<int> level;
    int _top = -1;

};

#endif
