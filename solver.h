#ifndef __SOLVER_H
#define __SOLVER_H

#include "parser.h"
#include <vector>
using namespace std;


class solver {
public:
    int maxVarIndex;
    vector< vector<int> > clauses;
    vector<bool> assignment;

    void init(const char *filename);

    void solve(int=1);
};

#endif
