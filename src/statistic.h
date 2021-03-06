#ifndef __STATISTIC_H
#define __STATISTIC_H

#include <algorithm>
#include <sys/time.h>
using namespace std;


struct Statistic {

    int preLearntAssignment = 0;
    int preLearntClause = 0;
    int preEliminateCls = 0;
    int preEliminateLit = 0;
    int backtrackNum = 0;
    int maxDepth = 0;
    int learnAssignment = 0;
    int learnCls = 0;
    int maxLearntSz = 0;
    int decision = 0;
    int propagation = 0;
    int removedLit = 0;
    long long totalLearntSz = 0;
    int maxJumpBack = 0;
    int restartTime = 0;
    double preprocessTime = 0;
    bool stopped = false;
    struct timeval start, end;

    // Init and start counter
    inline void init() {
        *this = Statistic();
        gettimeofday(&start, 0);
        gettimeofday(&end, 0);
    }

    inline void update(const Statistic &rth) {
        preLearntAssignment += rth.preLearntAssignment;
        preLearntClause += rth.preLearntClause;
        preEliminateCls += rth.preEliminateCls;
        preEliminateLit += rth.preEliminateLit;
        backtrackNum += rth.backtrackNum;
        maxDepth = max(maxDepth, rth.maxDepth);
        learnAssignment += rth.learnAssignment;
        learnCls += rth.learnCls;
        maxLearntSz = max(maxLearntSz, rth.maxLearntSz);
        decision += rth.decision;
        propagation += rth.propagation;
        removedLit += rth.removedLit;
        totalLearntSz += rth.totalLearntSz;
        maxJumpBack = max(maxJumpBack, rth.maxJumpBack);
        preprocessTime += rth.preprocessTime;
        restartTime += rth.restartTime;
    }

    // Stop counter
    inline void stopTimer() {
        stopped = true;
        gettimeofday(&end, 0);
    }

    // Return actual elapsed time(not cpu time) in seconds
    inline double elapseTime() {
        if( !stopped )
            gettimeofday(&end, 0);
        int sec = end.tv_sec - start.tv_sec;
        int usec = end.tv_usec - start.tv_usec;
        return sec + (usec/1000000.0);
    }

};

#endif
