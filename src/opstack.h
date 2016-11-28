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
    vector<op> stk;

    opStack() {}
    opStack(int n)
    :stk(n), idx(n), level(n) {
        level[0] = -1;
    }
    
    inline int getVal(int id) const {
        if( idx[id] > _top || stk[idx[id]].var != id )
            return 2;
        return stk[idx[id]].val;
    }
    inline op& topNext() {
        // Return variable on decision or empty slot
        return stk[_top+1];
    }
    inline void set(int id, int val, int lv) {
        // Set id = val@lv
        ++_top;
        stk[_top].var = id;
        stk[_top].val = val;
        idx[id] = _top;
        level[lv] = _top;
    }
    inline void backToLevel(int lv) {
        // Move top pointer to that level's last assignment
        _top = level[lv];
    }


protected:

    vector<int> idx;
    vector<int> level;
    int _top = -1;

};

#endif
