#ifndef __LAZYTABLE_H
#define __LAZYTABLE_H

#include <cstdlib>
#include <vector>
using namespace std;


struct Lazytable {

public:

    vector<int> table;

    inline void init(int sz) {
        table = vector<int>(sz+4);
        timestamp = vector<int>(sz+4, -1);
        nowT = 0;
    }

    inline void clear() {
        ++nowT;
    }

    inline int get(int id) {
        // Return -1 if unset yet
        int eq = (timestamp[id] == nowT);
        return table[id] * eq - !eq;
    }

    inline void set(int id, unsigned int val) {
        table[id] = val;
        timestamp[id] = nowT;
    }


protected:

    vector<int> timestamp;
    int nowT = 0;

};

#endif
