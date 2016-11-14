#include "solver.h"

void solver::init(const char *filename) {
    parse_DIMACS_CNF(clauses, maxVarIndex, filename);
    assignment = vector<bool>(maxVarIndex+4, 0);
}

void solver::solve(int nowAt) {
    if( nowAt > maxVarIndex ) {
        for(auto &cls : clauses) {
            bool clsOk = false;
            for(auto &v : cls) {
                if( (v>0 && assignment[v]) || (v<0 && !assignment[-v]) ) {
                    clsOk = true;
                    break;
                }
            }
            if( !clsOk ) return;
        }
        assignment[0] = true;
        return;
    }
    assignment[nowAt] = false;
    solve(nowAt+1);
    if( assignment[0] ) return;
    assignment[nowAt] = true;
    solve(nowAt+1);
}
