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
    puts("  -no-brain: use no brain recursive brute force");
}


int main(int argc, const char *argv[]) {
    if( argc < 2 ) {
        helpMessage();
        exit(0);
    }

    solver yasat;

    // Parse input parameter
    for(int i=1, init=false; i<argc; ++i)
        if( argv[i][0] != '-' && !init ) {
            init = true;
            yasat.init(argv[i]);
        }
        else {
            helpMessage();
            exit(0);
        }


    yasat.solve();


    // Print result
    if( yasat.assignment[0] ) {
        puts("s SATISFIABLE");
        putchar('v');
        for(int i=1; i<=yasat.maxVarIndex; ++i)
            printf(" %d", yasat.assignment[i] ? i : -i);
        puts(" 0");
    }
    else {
        puts("s UNSATISFIABLE");
    }
    return 0;
}
