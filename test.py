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

for dirname, dirnames, filenames in os.walk(sys.argv[1]):
    for filename in filenames:
        if filename.find('.cnf') == -1:
            continue
        TESTNUM = TESTNUM + 1
        print('=============================')
        print(os.path.join(dirname, filename))
        proc = subprocess.Popen(['./yasat', '-v', os.path.join(dirname, filename)],
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
        print('Time for SAT solver', nowTime, 'sec')
        print('Backtrack Num', nowBacktrackNum)
        MAXTIME = max(MAXTIME, nowTime)
        TOTALTIME = TOTALTIME + nowTime
        MAXBACKTRACKNUM = max(MAXBACKTRACKNUM, nowBacktrackNum)
        TOTALBACKTRACKNUM = TOTALBACKTRACKNUM + nowBacktrackNum

print('=============================')
print('SAT', SAT, '/ UNSAT', UNSAT, '/ ERROR', ERROR)
print('\nTime for SAT Solver')
print('   avg', TOTALTIME / TESTNUM, 'sec')
print('   max', MAXTIME, 'sec')
print('\nBacktrack Num')
print(' total', TOTALBACKTRACKNUM)
print('   max', MAXBACKTRACKNUM)
