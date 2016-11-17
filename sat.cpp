#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
#include "solver.h"
using namespace std;


void helpMessage() {
    puts("Unexpected parameter");
    puts("./yasat [options] testcase.cnf");
    puts("");
    puts("options: ");
    puts("  -statistic: print statitic result to stderr");
    puts("  -no: don't use any heuristic");
    puts("  -mom: (default) use MOM branching heuristic");
}


int main(int argc, const char *argv[]) {
    if( argc < 2 ) {
        helpMessage();
        exit(1);
    }

    solver yasat;
    bool statistic = false;
    int mode = solver::HEURISTIC_MOM;

    // Parse input parameter
    int srcid = 0;
    for(int i=1, init=false; i<argc; ++i)
        if( argv[i][0] != '-' && !init ) {
            init = true;
            srcid = i;
            yasat.init(argv[i]);
        }
        else if( strcmp(argv[i], "-statistic") == 0 ) {
            statistic = true;
        }
        else if( strcmp(argv[i], "-no") == 0 ) {
            mode = solver::HEURISTIC_NO;
        }
        else if( strcmp(argv[i], "-mom") == 0 ) {
            mode = solver::HEURISTIC_MOM;
        }
        else {
            helpMessage();
            exit(1);
        }

    if( !srcid ) {
        helpMessage();
        exit(1);
    }


    yasat.solve(mode);


    // Print result
    if( statistic ) {
        fprintf(stderr, "================ statistic ================\n");
        fprintf(stderr, "Time on SAT solver: %.3f sec\n", yasat.statistic.elapseTime());
        fprintf(stderr, "Backtrack num     : %d\n", yasat.statistic.backtrackNum);
        fprintf(stderr, "Max depth         : %d\n", yasat.statistic.maxDepth);
        fprintf(stderr, "===========================================\n");
    }

    if( yasat.sat ) {
        puts("s SATISFIABLE");
        putchar('v');
        for(int i=1; i<=yasat.maxVarIndex; ++i)
            printf(" %d", yasat.var.getVal(i) ? i : -i);
        puts(" 0");
    }
    else {
        puts("s UNSATISFIABLE");
    }
    return 0;
}
