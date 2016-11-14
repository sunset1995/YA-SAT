#include <cstdio>
#include <cstdlib>
#include <vector>
#include "parser.h"
using namespace std;

void helpMessage() {
    puts("Unknown expected parameter");
    puts("./yasat [options] testcase.cnf");
}

int main(int argc, const char *argv[]) {
    if( argc < 2 ) {
        helpMessage();
        exit(0);
    }

    int maxVarIndex;
    vector< vector<int> > clauses;
    for(int i=1; i<argc; ++i)
        if( argv[i][0] != '-' && clauses.empty() )
            parse_DIMACS_CNF(clauses, maxVarIndex, argv[i]);

    printf("maxVarIndex %d\n", maxVarIndex);
    for(auto &cls : clauses) {
        for(auto &v : cls)
            printf("%d ", v);
        puts("");
    }
    return 0;
}
