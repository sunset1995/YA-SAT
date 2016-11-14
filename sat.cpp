#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
#include "parser.h"
using namespace std;

// Solver options
bool useNoBrain = true;

// CNF Database
int maxVarIndex;
vector< vector<int> > clauses;
vector<bool> assignment;

void helpMessage() {
    puts("Unknown expected parameter");
    puts("./yasat [options] testcase.cnf");
    puts("");
    puts("options: ");
    puts("  -no-brain: use no brain recursive brute force");
}

void noBrain(int id) {
    if( id > maxVarIndex ) {
        for(auto &cls : clauses) {
            bool clsSat = false;
            for(auto &v : cls)
                if( (v > 0 && assignment[v]) ||
                        (v < 0 && !assignment[-v]) ) {
                    clsSat = true;
                    break;
                }
            if( !clsSat ) return;
        }
        assignment[0] = true;
        return;
    }
    assignment[id] = false;
    noBrain(id+1);
    if( assignment[0] ) return;
    assignment[id] = true;
    noBrain(id+1);
}

void noBrain() {
    noBrain(1);
}

int main(int argc, const char *argv[]) {
    if( argc < 2 ) {
        helpMessage();
        exit(0);
    }

    // Parse input parameter
    for(int i=1; i<argc; ++i)
        if( argv[i][0] != '-' && clauses.empty() ) 
            parse_DIMACS_CNF(clauses, maxVarIndex, argv[i]);
        else if( strstr(argv[i], "-no-brain") )
            useNoBrain = true;
        else {
            helpMessage();
            exit(0);
        }


    // Solve
    assignment = vector<bool>(maxVarIndex + 4, 0);
    if( useNoBrain )
        noBrain();


    // Print result
    if( assignment[0] ) {
        puts("s SATISFIABLE");
        putchar('v');
        for(int i=1; i<=maxVarIndex; ++i)
            printf(" %d", assignment[i] ? i : -i);
        puts(" 0");
    }
    else {
        puts("s UNSATISFIABLE");
    }
    return 0;
}
