import re
import os
import sys
import subprocess
from natsort import natsort_keygen, ns

solver = './yasat' if len(sys.argv)<2 else sys.argv[1]

print(" ALL UNSAT ".center(50, "="))
basename = 'benchmarks/UNSAT'
_, dirnames, _ = tuple(os.walk(basename))[0]
dirnames.sort(key=lambda x: natsort_keygen(alg=ns.IGNORECASE)(x))
for scale in dirnames:
    print("")
    print(scale.center(50, "-"), flush=True)
    _, _, problems = tuple(os.walk('%s/%s' % (basename, scale)))[0]

    SAT = 0
    UNSAT = 0
    ERROR = 0

    title = []
    statistic = []

    for problem in problems:

        # Run only .cnf files
        if problem.find('.cnf') == -1:
            continue
        proc = subprocess.Popen([
            solver, '-stdout', '-statistic',
            "%s/%s/%s" % (basename, scale, problem)],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,)
        try:
            outs, errs = proc.communicate(input=None, timeout=1200)
        except subprocess.TimeoutExpired:
            proc.kill()
            outs, errs = proc.communicate()
            TIMEOUT += 1
            print('TIMEOUT\n')
            continue
        result = str(outs)


        # Read SAT/UNSAT/ERROR
        if proc.returncode != 0:
            ERROR = ERROR + 1
            continue
        if result.find('UNSATISFIABLE') != -1:
            UNSAT = UNSAT + 1
        else:
            SAT = SAT + 1


        # Read statistic
        nowStat = str(errs).split('\\n')

        # Check init
        if len(statistic)==0:
            title = [''] * len(nowStat)
            statistic = [[0.0, 0.0] for _ in range(len(nowStat))]
            for i in range(1, len(nowStat)-2):
                title[i] = nowStat[i].split(':')[0]

        # Parse statistic
        for i in range(1, len(nowStat)-2):
            nowVal = float(re.findall('[-+]?\d*\.\d+|[-+]?\d+', nowStat[i])[0])
            statistic[i][0] += nowVal
            statistic[i][1] = max(nowVal, statistic[i][1])

    print('SAT', SAT, '/ UNSAT', UNSAT, '/ ERROR', ERROR)
    for i in range(1, len(statistic)-2):
        print("%s : %10.2f / %10.2f" % (title[i], statistic[i][0]/(SAT + UNSAT), statistic[i][1]))

print("".center(50, "="))
