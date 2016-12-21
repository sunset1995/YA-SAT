#include "heap.h"

void VarHeap::init(int maxVarIndex) {
    fullSz = maxVarIndex;
    sz = 0;
    arr = vector<HeapEntry>(fullSz + 4);
    mapping = vector<int>(fullSz + 4);
    signCnt = vector<int>(fullSz + 4, 0);
    for(int i=1; i<=maxVarIndex; ++i) {
        arr[i].var = i;
        mapping[i] = i;
    }
}

void VarHeap::upward(int id) {
    while( id>1 ) {
        int fid = id>>1;
        if( arr[id].pri > arr[fid].pri )
            swapEntry(id, fid);
        else
            break;
        id = fid;
    }
}

void VarHeap::downward(int id) {
    while( true ) {
        int lcid = id<<1;
        int rcid = (id<<1) + 1;
        double lpri = (lcid <= sz ? arr[lcid].pri : -1.0);
        double rpri = (rcid <= sz ? arr[rcid].pri : -1.0);
        if( arr[id].pri >= lpri && arr[id].pri >= rpri )
            break;
        else if( lpri > rpri ) {
            swapEntry(id, lcid);
            id = lcid;
        }
        else {
            swapEntry(id, rcid);
            id = rcid;
        }
    }
}
