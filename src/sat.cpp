#include <cstdio>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <thread>
#include <mutex>
#include "solver.h"
using namespace std;


solver yasat;
mutex locker;
int solveDone = 0;

void helpMessage() {
    puts("Unexpected parameter");
    puts("./yasat [options] testcase.cnf");
    puts("");
    puts("options: ");
    puts("  -statistic: print statitic result to stderr");
    puts("  -stdout   : print result to stdout instead of file");
    puts("  -no       : init var randomly");
    puts("  -mom      : (default) init var score by MOM");
    puts("  -ruby     : use ruby sequence to restart");
    puts("  -novsids  : disable Variable State Independent Decaying Sum heuristic");
    puts("  -nomulti  : disable multi-thread running all method concurrently");
}


// Worker for -xxx
struct WorkerAttr {
    const char *file;
    int mode;
    WorkerAttr(const char *file, int mode)
    :file(file), mode(mode) {}
};
void xxxWorker(WorkerAttr attr) {
    solver worker = yasat;

    worker.solve(attr.mode);

    lock_guard<std::mutex> lock(locker);
    if( !solveDone ) {
        solveDone = 1;
        yasat = move(worker);
    }
}


int main(int argc, const char *argv[]) {
    if( argc < 2 ) {
        helpMessage();
        exit(1);
    }

    srand(time(NULL));

    bool statistic = false;
    long mode = solver::HEURISTIC_VSIDS | solver::HEURISTIC_MOM_INIT;

    // Parse input parameter
    int srcid = 0, toStdout = 0, xxx = 1;
    for(int i=1, init=false; i<argc; ++i)
        if( argv[i][0] != '-' && !init ) {
            init = true;
            srcid = i;
            yasat.init(argv[srcid]);
        }
        else if( strcmp(argv[i], "-statistic") == 0 ) {
            statistic = true;
        }
        else if( strcmp(argv[i], "-stdout") == 0 ) {
            toStdout = 1;
        }
        else if( strcmp(argv[i], "-no") == 0 ) {
            mode &= ~solver::HEURISTIC_MOM_INIT;
            mode |= solver::HEURISTIC_NO_INIT;
            xxx = 0;
        }
        else if( strcmp(argv[i], "-mom") == 0 ) {
            mode &= ~solver::HEURISTIC_NO_INIT;
            mode |= solver::HEURISTIC_MOM_INIT;
            xxx = 0;
        }
        else if( strcmp(argv[i], "-novsids") == 0 ) {
            mode &= ~solver::HEURISTIC_VSIDS;
        }
        else if( strcmp(argv[i], "-nomulti") == 0 ) {
            xxx = 0;
        }
        else if( strcmp(argv[i], "-ruby") == 0 ) {
            mode |= solver::RESTART_RUBY;
            xxx = 0;
        }
        else {
            helpMessage();
            exit(1);
        }

    if( !srcid ) {
        helpMessage();
        exit(1);
    }


    if( xxx && yasat.size() > 150 ) {

        thread mom(xxxWorker, WorkerAttr(
            argv[srcid],
            solver::HEURISTIC_VSIDS | solver::HEURISTIC_MOM_INIT));
        thread ruby(xxxWorker, WorkerAttr(
            argv[srcid],
            solver::HEURISTIC_VSIDS | solver::HEURISTIC_MOM_INIT | solver::RESTART_RUBY));
        thread no(xxxWorker, WorkerAttr(
            argv[srcid],
            solver::HEURISTIC_VSIDS | solver::HEURISTIC_NO_INIT));
        if( mom.joinable() )
            mom.join();
        if( ruby.joinable() )
            ruby.join();
        if( no.joinable() )
            no.join();

    }
    else {
        yasat.solve(mode);
    }


    // Print result
    if( statistic ) {
        fprintf(stderr, "================ statistic ================\n");
        fprintf(stderr, "Time on SAT solver: %.3f sec\n", yasat.statistic.elapseTime());
        fprintf(stderr, "Backtrack num     : %d\n", yasat.statistic.backtrackNum);
        fprintf(stderr, "Max depth         : %d\n", yasat.statistic.maxDepth);
        fprintf(stderr, "Learnt clause     : %d\n", yasat.statistic.learnCls);
        fprintf(stderr, "Avg Learnt Cls Sz : %.2f\n", yasat.statistic.learnCls ? double(yasat.statistic.totalLearntSz)/yasat.statistic.learnCls : 0);
        fprintf(stderr, "Max Learnt Cls Sz : %d\n", yasat.statistic.maxLearntSz);
        fprintf(stderr, "Learnt assignment : %d\n", yasat.statistic.learnAssignment);
        fprintf(stderr, "Max JumpBack      : %d\n", yasat.statistic.maxJumpBack);
        fprintf(stderr, "Restart           : %d\n", yasat.statistic.restartTime);
        fprintf(stderr, "===========================================\n");
    }

    if( !toStdout ) {
        fflush(stdout);
        char filename[100];
        strcpy(filename, argv[srcid]);
        int len = strlen(filename);
        if( len>=4  && filename[len-4]=='.' && filename[len-3] == 'c' && 
                filename[len-2] == 'n' && filename[len-1]=='f' ) {
            filename[len-3] = 's';
            filename[len-2] = 'a';
            filename[len-1] = 't';
        }
        else {
            filename[len] = '.';
            filename[len+1] = 's';
            filename[len+2] = 'a';
            filename[len+3] = 't';
            filename[len+4] = '\0';
        }
        freopen(filename, "w", stdout);
    }

    vector<int> result = yasat.result();
    if( result[0] ) {
        puts("s SATISFIABLE");
        putchar('v');
        for(int i=1; i<result.size(); ++i)
            printf(" %d", result[i]);
        puts(" 0");
    }
    else {
        puts("s UNSATISFIABLE");
    }
    return 0;
}
