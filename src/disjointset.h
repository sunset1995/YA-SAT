#ifndef __DISJOINTSET_H
#define __DISJOINTSET_H

#include <vector>
using namespace std;


class DisjointSet {

public:

    inline void init(int n) {
        root.resize(n);
        for(int i=0; i<n; ++i)
            root[i] = i;
    }

    inline bool sameSet(int a, int b) {
        return findRoot(a) == findRoot(b);
    }

    inline void unionSet(int a, int b) {
        a = findRoot(a);
        b = findRoot(b);
        if( a!=b )
            root[b] = a;
    }

    int findRoot(int id) {
        return id==root[id] ? id : (root[id] = findRoot(root[id]));
    }


protected:
    
    // Each node's set id
    vector<int> root;

};

#endif
