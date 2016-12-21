#ifndef __HEAP_H
#define __HEAP_H

#include <vector>
#include <algorithm>
using namespace std;


// Heap maintaining priority order of all variables
// Priority must be positive floating point
class VarHeap {

public:
    void init(int maxVarIndex);
    inline void increaseInitPri(int var, double pri);
    inline void heapify();
    inline int size();
    inline int top();
    inline void pop();
    inline void restore(int var);
    inline void increasePri(int var, double pri);

//protected:
    struct HeapEntry {
        double pri = 0;
        int var = 0;
        HeapEntry() 
        :pri(0), var(0) {};
        HeapEntry(double pri, int var)
        :pri(pri), var(var) {}
    };
    
    // Array as heap
    vector<HeapEntry> arr;
    vector<int> signCnt;
    int sz, fullSz;

    // Mapping var to index in arr
    vector<int> mapping;
    inline void swapEntry(int a, int b);

    // Function maintaining heap property
    void upward(int id);
    void downward(int id);

};


inline void VarHeap::increaseInitPri(int var, double pri) {
    arr[mapping[var]].pri += pri;
}

inline int VarHeap::size() {
    return sz;
}

inline int VarHeap::top() {
    return arr[1].var;
}

inline void VarHeap::swapEntry(int aid, int bid) {
    mapping[arr[aid].var] = bid;
    mapping[arr[bid].var] = aid;
    swap(arr[aid], arr[bid]);
}

inline void VarHeap::heapify() {
    while( sz < fullSz )
        upward(++sz);
}

inline void VarHeap::pop() {
    //int var = arr[1].var;
    swapEntry(1, sz--);
    downward(1);
    //fprintf(stderr, "=========================> pop %d(%d)\n", var, mapping[var]);
}

inline void VarHeap::restore(int var) {
    int id = mapping[var];
    //fprintf(stderr, "try restore %d(%d)\n", var, id);
    if( id <= sz )
        return;
    ++sz;
    if( var != arr[sz].var )
        swapEntry(id, sz);
    //fprintf(stderr, "=========================> restore %d\n", arr[sz].var);
    upward(sz);
}

inline void VarHeap::increasePri(int var, double pri) {
    int id = mapping[var];
    arr[id].pri += pri;
    upward(id);
}


#endif
