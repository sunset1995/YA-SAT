import re
import os
import sys
import subprocess

if len(sys.argv) != 2:
    print('python3 test.py testbenchDir')
    exit(0)

SAT = 0
UNSAT = 0
ERROR = 0

title = []
statistic = []

for dirname, dirnames, filenames in os.walk(sys.argv[1]):
    for filename in filenames:
        
        # Run only .cnf files
        if filename.find('.cnf') == -1:
            continue

        print(os.path.join(dirname, filename).center(70, "-"))
        proc = subprocess.Popen([
            './yasat', '-stdout', '-statistic',
            os.path.join(dirname, filename)],
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

        # Read statistic
        nowStat = str(proc.stderr.read()).split('\\n')

        # Check init
        if len(statistic)==0:
            title = [''] * len(nowStat)
            statistic = [[0.0, 0.0] for _ in range(len(nowStat))]
            for i in range(1, len(nowStat)-2):
                title[i] = nowStat[i].split(':')[0]

        # Parse statistic
        for i in range(1, len(nowStat)-2):
            print(nowStat[i])
            nowVal = float(re.findall('[-+]?\d*\.\d+|[-+]?\d+', nowStat[i])[0])
            statistic[i][0] += nowVal
            statistic[i][1] = max(nowVal, statistic[i][1])

        print("")


print(" statistic ".center(70, "-"))
print('SAT', SAT, '/ UNSAT', UNSAT, '/ ERROR', ERROR)
for i in range(1, len(statistic)-2):
    print("%s : %10.2f / %10.2f" % (title[i], statistic[i][0]/(SAT + UNSAT), statistic[i][1]))
print("".center(70, "="))
