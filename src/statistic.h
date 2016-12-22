#ifndef __STATISTIC_H
#define __STATISTIC_H

#include <sys/time.h>


struct Statistic {

    int backtrackNum = 0;
    int maxDepth = 0;
    int learnAssignment = 0;
    int learnCls = 0;
    int maxLearntSz = 0;
    long long totalLearntSz = 0;
    int maxJumpBack = 0;
    int restartTime = 0;
    struct timeval start, end;

    // Init and start counter
    inline void init() {
        backtrackNum = maxDepth = learnAssignment = learnCls = 0;
        maxLearntSz = 0;
        gettimeofday(&start, 0);
        gettimeofday(&end, 0);
    }

    inline void update(const Statistic &rth) {
        backtrackNum += rth.backtrackNum;
        maxDepth = max(maxDepth, rth.maxDepth);
        learnAssignment += rth.learnAssignment;
        learnCls += rth.learnCls;
        maxLearntSz = max(maxLearntSz, rth.maxLearntSz);
        totalLearntSz += rth.totalLearntSz;
        maxJumpBack = max(maxJumpBack, rth.maxJumpBack);
        restartTime += rth.restartTime;
    }

    // Stop counter
    inline void stopTimer() {
        gettimeofday(&end, 0);
    }

    // Return actual elapsed time(not cpu time) in seconds
    inline double elapseTime() {
        int sec = end.tv_sec - start.tv_sec;
        int usec = end.tv_usec - start.tv_usec;
        return sec + (usec/1000000.0);
    }

};

#endif
