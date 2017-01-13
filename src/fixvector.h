#ifndef __FIXVECTOR_H
#define __FIXVECTOR_H

#include <vector>
using namespace std;


template <typename T>
class FixVector {

public:

    vector<T> _element;
    int _top;

    inline void init(int n) {
        _element = vector<T>(n);
        _top = -1;
    }

    inline void clear() {
        _top = -1;
    }

    inline int size() const {
        return _top + 1;
    }

    inline void resize(int sz) {
        _top = sz-1;
    }

    inline void push_back(const T &ele) {
        _element[++_top] = ele;
    }

    inline T& back() {
        return _element[_top];
    }

    operator vector<T>() const {
        vector<T> ret(size());
        for(int i=size()-1; i>=0; --i)
            ret[i] = _element[i];
        return ret;
    }

    T& operator [] (int idx) {
        return _element[idx];
    }

};


#endif
