#include <cstdio>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <thread>
#include <mutex>
#include "statistic.h"
#include "solver.h"
using namespace std;


solver yasat;
mutex locker;
int solveDone = 0;
vector<int> result;
Statistic statistic;

void helpMessage() {
    puts("Unexpected parameter");
    puts("./yasat [options] testcase.cnf");
    puts("");
    puts("options: ");
    puts("  -statistic: print statitic result to stderr");
    puts("  -stdout   : print result to stdout instead of file");
    puts("  -nomom    : disable init var score by MOM");
    puts("  -noruby   : disable ruby sequence to restart");
    puts("  -stupid   : restart rapidly stupidly");
    puts("  -novsids  : disable Variable State Independent Decaying Sum heuristic");
    puts("  -multi    : enable multi-thread running all method concurrently");
    puts("  -phase    : phase saving");
    puts("  -rand     : add random factor in VSISD");
    puts("  -pos      : pick positive phase while tie");
    puts("  -pre      : enable advance preprocess");
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
        result = worker.result();
        statistic = worker.statistic;
    }
}


int main(int argc, const char *argv[]) {
    if( argc < 2 ) {
        helpMessage();
        exit(1);
    }

    srand(time(NULL));

    bool stat = false;
    long mode = solver::default_mode;

    // Parse input parameter
    int srcid = 0, toStdout = 0, xxx = 0;
    for(int i=1, init=false; i<argc; ++i)
        if( argv[i][0] != '-' && !init ) {
            init = true;
            srcid = i;
            yasat.init(argv[srcid]);
        }
        else if( strcmp(argv[i], "-statistic") == 0 )
            stat = true;
        else if( strcmp(argv[i], "-stdout") == 0 )
            toStdout = 1;
        else if( strcmp(argv[i], "-nomom") == 0 ) {
            mode &= ~solver::HEURISTIC_MOM_INIT;
            mode |= solver::HEURISTIC_NO_INIT;
        }
        else if( strcmp(argv[i], "-novsids") == 0 )
            mode &= ~solver::HEURISTIC_VSIDS;
        else if( strcmp(argv[i], "-multi") == 0 )
            xxx = 1;
        else if( strcmp(argv[i], "-noruby") == 0 )
            mode &= ~solver::RESTART_RUBY;
        else if( strcmp(argv[i], "-stupid") == 0 )
            mode |= solver::RESTART_STUPID;
        else if( strcmp(argv[i], "-pre") == 0 )
            mode |= solver::PREPROCESS;
        else if( strcmp(argv[i], "-rand") == 0 )
            mode |= solver::RAND;
        else if( strcmp(argv[i], "-pos") == 0 )
            mode |= solver::POS;
        else if( strcmp(argv[i], "-phase") == 0 )
            mode |= solver::PHASESAVING;
        else {
            helpMessage();
            exit(1);
        }

    if( !srcid ) {
        helpMessage();
        exit(1);
    }


    if( xxx ) {

        thread mom(xxxWorker, WorkerAttr(
            argv[srcid],
            solver::HEURISTIC_VSIDS | solver::HEURISTIC_MOM_INIT));
        thread ruby(xxxWorker, WorkerAttr(
            argv[srcid],
            solver::HEURISTIC_VSIDS | solver::HEURISTIC_MOM_INIT | solver::RESTART_RUBY));
        thread rubyPos(xxxWorker, WorkerAttr(
            argv[srcid],
            solver::HEURISTIC_VSIDS | solver::HEURISTIC_MOM_INIT | solver::RESTART_RUBY | solver::POS));
        thread rubyRand(xxxWorker, WorkerAttr(
            argv[srcid],
            solver::HEURISTIC_VSIDS | solver::HEURISTIC_MOM_INIT | solver::RESTART_RUBY | solver::PHASESAVING));
        if( mom.joinable() )
            mom.join();
        if( ruby.joinable() )
            ruby.join();
        if( rubyPos.joinable() )
            rubyPos.join();
        if( rubyRand.joinable() )
            rubyRand.join();

    }
    else {
        yasat.solve(mode);
        result = yasat.result();
        statistic = yasat.statistic;
    }


    // Print result
    if( stat ) {
        fprintf(stderr, "================ statistic ================\n");
        fprintf(stderr, "Time on SAT solver: %.3f sec (preprocess %.2f%%)\n", statistic.elapseTime()
            ,100.0*statistic.preprocessTime/statistic.elapseTime());
        fprintf(stderr, "Pre learnt assign : %d\n", statistic.preLearntAssignment);
        fprintf(stderr, "Pre learnt clause : %d\n", statistic.preLearntClause);
        fprintf(stderr, "Pre eliminate cls : %d\n", statistic.preEliminateCls);
        fprintf(stderr, "Pre eliminate lit : %d\n", statistic.preEliminateLit);
        fprintf(stderr, "Decision      num : %d\n", statistic.decision);
        fprintf(stderr, "Propagation   num : %d\n", statistic.propagation);
        fprintf(stderr, "Propagation speed : %.f /sec\n", statistic.propagation/statistic.elapseTime());
        fprintf(stderr, "Removed lit   num : %d\n", statistic.removedLit);
        fprintf(stderr, "Backtrack     num : %d\n", statistic.backtrackNum);
        fprintf(stderr, "Max depth         : %d\n", statistic.maxDepth);
        fprintf(stderr, "Learnt clause     : %d\n", statistic.learnCls);
        fprintf(stderr, "Avg Learnt Cls Sz : %.2f\n", statistic.learnCls ? double(statistic.totalLearntSz)/statistic.learnCls : 0);
        fprintf(stderr, "Max Learnt Cls Sz : %d\n", statistic.maxLearntSz);
        fprintf(stderr, "Learnt assignment : %d\n", statistic.learnAssignment);
        fprintf(stderr, "Max JumpBack      : %d\n", statistic.maxJumpBack);
        fprintf(stderr, "Restart           : %d\n", statistic.restartTime);
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
