import re
import os
import sys
import subprocess

print('======== SAT ========')
basename = 'benchmarks/SAT'
_, dirnames, _ = tuple(os.walk(basename))[0]
for scale in dirnames:
    print("\n======== %s ========" % scale)
    _, _, problems = tuple(os.walk('%s/%s' % (basename, scale)))[0]

    SAT = 0
    UNSAT = 0
    ERROR = 0
    MAXTIME = 0
    TOTALTIME = 0
    MAXBACKTRACKNUM = 0
    TOTALBACKTRACKNUM = 0
    MAXDEPTH = 0
    TOTALDEPTH = 0

    for problem in problems:
        if problem.find('.cnf') == -1:
            continue
        proc = subprocess.Popen([
            './yasat', '-stdout', '-statistic',
            "%s/%s/%s" % (basename, scale, problem)],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,)
        proc.wait()
        result = str(proc.stdout.read())
        if proc.returncode != 0:
            ERROR = ERROR + 1
            continue
        if result.find('UNSATISFIABLE') != -1:
            UNSAT = UNSAT + 1
        else:
            SAT = SAT + 1
        statistic = str(proc.stderr.read()).split('\\n')
        nowTime = float(re.findall('[-+]?\d*\.\d+|[-+]?\d+', statistic[1])[0])
        nowBacktrackNum = [int(v) for v in statistic[2].split() if v.isdigit()][0]
        nowDepth = [int(v) for v in statistic[3].split() if v.isdigit()][0]
        MAXTIME = max(MAXTIME, nowTime)
        TOTALTIME = TOTALTIME + nowTime
        MAXBACKTRACKNUM = max(MAXBACKTRACKNUM, nowBacktrackNum)
        TOTALBACKTRACKNUM = TOTALBACKTRACKNUM + nowBacktrackNum
        MAXDEPTH = max(MAXDEPTH, nowDepth)
        TOTALDEPTH = TOTALDEPTH + nowDepth

    print('SAT', SAT, '/ UNSAT', UNSAT, '/ ERROR', ERROR)
    print('\nTime for SAT Solver')
    print('   avg %.3f sec' % (TOTALTIME / (SAT + UNSAT)))
    print('   max %.3f sec' % (MAXTIME))
    print('\nBacktrack Num')
    print('   avg', int(TOTALBACKTRACKNUM/(SAT + UNSAT)))
    print('   max', MAXBACKTRACKNUM)
    print('\nMax Depth')
    print('   avg', int(TOTALDEPTH/(SAT + UNSAT)))
    print('   max', MAXDEPTH)
