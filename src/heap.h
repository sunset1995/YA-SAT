#ifndef __HEAP_H
#define __HEAP_H

#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cmath>
using namespace std;


// Heap maintaining priority order of all variables
// Priority must be positive floating point
class VarHeap {

public:
    void init(int maxVarIndex);
    inline void increaseInitPri(int var, double pri, int sign);
    inline void heapify();
    inline int size();
    inline int top();
    inline void pop();
    inline int litBalance(int var);
    inline void restore(int var);
    inline void increasePri(int var, double pri, int sign);
    inline void decayAll();
    constexpr static const double decayFactor = 0.95;

protected:
    struct HeapEntry {
        double pri = 0;
        int var = 0;
        int lastEvalT = 0;
        HeapEntry() 
        :pri(0), var(0), lastEvalT(0) {};
        HeapEntry(double pri, int var)
        :pri(pri), var(var), lastEvalT(0) {}
    };
    int nowT = 0;
    
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

    // Helper function
    inline double getPri(int id);
    inline void addPri(int id, double pri);

};


inline void VarHeap::increaseInitPri(int var, double pri, int sign=-1) {
    arr[mapping[var]].pri += pri;
    if( sign!=-1 )
        signCnt[var] += (sign ? 1 : -1);
}

inline int VarHeap::size() {
    return sz;
}

inline int VarHeap::top() {
    return arr[1].var;
}

inline void VarHeap::decayAll() {
    ++nowT;
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
    swapEntry(1, sz--);
    downward(1);
}

inline int VarHeap::litBalance(int var) {
    return signCnt[var];
}

inline void VarHeap::restore(int var) {
    int id = mapping[var];
    if( id <= sz )
        return;
    ++sz;
    if( var != arr[sz].var )
        swapEntry(id, sz);
    upward(sz);
}

inline void VarHeap::increasePri(int var, double pri, int sign=-1) {
    int id = mapping[var];
    addPri(id, pri);
    if( sign!=-1 )
        signCnt[id] += (sign ? 1 : -1);
    if( id <= sz )
        upward(id);
}

inline double VarHeap::getPri(int id) {
    if( arr[id].lastEvalT != nowT ) {
        arr[id].pri *= pow(decayFactor, nowT - arr[id].lastEvalT);
        arr[id].lastEvalT = nowT;
    }
    return arr[id].pri;
}

inline void VarHeap::addPri(int id, double pri) {
    arr[id].pri = getPri(id) + pri;
}


#endif
