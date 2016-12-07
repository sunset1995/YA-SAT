import re
import os
import sys
import subprocess

if len(sys.argv) != 2:
    print('python3 test.py testbenchDir')
    exit(0)

TESTNUM = 0
SAT = 0
UNSAT = 0
ERROR = 0
MAXTIME = 0
TOTALTIME = 0
MAXBACKTRACKNUM = 0
TOTALBACKTRACKNUM = 0
MAXDEPTH = 0
TOTALDEPTH = 0
MAXLEANRTCLS = 0
TOTALLEANRTCLS = 0
MAXLEANRTASS = 0
TOTALLEANRTASS = 0
MAXJUMPBACK = 0
TOTALJUMPBACK = 0
MAXLEANRTSZ = 0
TOTALLEANRTSZ = 0

for dirname, dirnames, filenames in os.walk(sys.argv[1]):
    for filename in filenames:
        if filename.find('.cnf') == -1:
            continue
        TESTNUM = TESTNUM + 1
        print('=============================')
        print(os.path.join(dirname, filename))
        proc = subprocess.Popen(['./yasat', '-stdout', '-statistic', os.path.join(dirname, filename)],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,)
        proc.wait()
        result = str(proc.stdout.read())
        if proc.returncode != 0:
            ERROR = ERROR + 1
            continue
        if result.find('UNSATISFIABLE') != -1:
            print('UNSAT')
            UNSAT = UNSAT + 1
        else:
            print('SAT')
            SAT = SAT + 1
        statistic = str(proc.stderr.read()).split('\\n')
        nowTime = float(re.findall('[-+]?\d*\.\d+|[-+]?\d+', statistic[1])[0])
        nowBacktrackNum = [int(v) for v in statistic[2].split() if v.isdigit()][0]
        nowDepth = [int(v) for v in statistic[3].split() if v.isdigit()][0]
        nowLearnCls = [int(v) for v in statistic[4].split() if v.isdigit()][0]
        nowLearnAss = [int(v) for v in statistic[5].split() if v.isdigit()][0]
        nowJumpBack = [int(v) for v in statistic[6].split() if v.isdigit()][0]
        nowLearntSz = [int(v) for v in statistic[7].split() if v.isdigit()][0]
        print('Time for SAT solver', nowTime, 'sec')
        print('Backtrack Num', nowBacktrackNum)
        print('Max depth', nowDepth)
        print('Learnt clause', nowLearnCls)
        print('Learnt assignment', nowLearnAss)
        print('Max jump back', nowJumpBack)
        print('Max learnt sz', nowLearntSz)
        MAXTIME = max(MAXTIME, nowTime)
        TOTALTIME = TOTALTIME + nowTime
        MAXBACKTRACKNUM = max(MAXBACKTRACKNUM, nowBacktrackNum)
        TOTALBACKTRACKNUM = TOTALBACKTRACKNUM + nowBacktrackNum
        MAXDEPTH = max(MAXDEPTH, nowDepth)
        TOTALDEPTH = TOTALDEPTH + nowDepth
        MAXLEANRTCLS = max(MAXLEANRTCLS, nowLearnCls)
        TOTALLEANRTCLS = TOTALLEANRTCLS + nowLearnCls
        MAXLEANRTASS = max(MAXLEANRTASS, nowLearnAss)
        TOTALLEANRTASS = TOTALLEANRTASS + nowLearnAss
        MAXJUMPBACK = max(MAXJUMPBACK, nowJumpBack)
        TOTALJUMPBACK = TOTALJUMPBACK + nowJumpBack
        MAXLEANRTSZ = max(MAXLEANRTSZ, nowLearntSz)
        TOTALLEANRTSZ = TOTALLEANRTSZ + nowLearntSz

print('=============================')
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
print('\nLearnt clause')
print('   avg', TOTALLEANRTCLS/(SAT + UNSAT))
print('   max', MAXLEANRTCLS)
print('\nLearnt assignment')
print('   avg', TOTALLEANRTASS/(SAT + UNSAT))
print('   max', MAXLEANRTASS)
print('\nMax jump back')
print('   avg', int(TOTALJUMPBACK/(SAT + UNSAT)))
print('   max', MAXJUMPBACK)
print('\nMax learnt sz')
print('   avg', int(TOTALLEANRTSZ/(SAT + UNSAT)))
print('   max', MAXLEANRTSZ)
