import re
import os
import sys
import subprocess
from natsort import natsort_keygen, ns

if len(sys.argv) < 2:
    print('python3 validator_all.py testbenchDir')
    exit(0)

solver = './yasat' if len(sys.argv)<3 else sys.argv[2]

SAT = 0
UNSAT = 0
ERROR = 0
TIMEOUT = 0


def check(cnfFile, result):

    assignment = set(str(result).split()[2:-1])

    ok = True
    clsok = False
    with open(cnfFile) as cnf:
        for line in cnf:
            clause = line.split()
            if len(clause)==0 or clause[0] == 'p' or clause[0] == 'c':
                continue
            for v in clause:
                if v == '0':
                    if not clsok:
                        ok = False
                    clsok = False
                elif v in assignment:
                    clsok = True

    return ok


for dirname, dirnames, filenames in os.walk(sys.argv[1]):

    filenames.sort(key=lambda x: natsort_keygen(alg=ns.IGNORECASE)(x))
    for filename in filenames:
        
        # Run only .cnf files
        if filename.find('.cnf') == -1:
            continue
        cnfFile = os.path.join(dirname, filename)
        print(os.path.join(filename).ljust(20, " "), end='')
        proc = subprocess.Popen([
            solver, '-stdout', '-statistic',
            cnfFile],
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

        if proc.returncode != 0:
            print('ERROR')
            ERROR = ERROR + 1
        elif result.find('UNSATISFIABLE') != -1:
            print('UNSAT')
            UNSAT = UNSAT + 1
        else:
            print('SAT', 'AC' if check(cnfFile, result) else 'WA')
            SAT = SAT + 1


print(" statistic ".center(70, "-"))
print('SAT', SAT, '/ UNSAT', UNSAT, '/ ERROR', ERROR, '/ TIMEOUT', TIMEOUT)
print("".center(70, "="))
