#ifndef __OPSTACK_H
#define __OPSTACK_H

#include <vector>
using namespace std;


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

#endif
