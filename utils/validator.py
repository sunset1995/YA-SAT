import os
import sys

assignment = set()

with open(sys.argv[2]) as sat:
    assignment = set(str(sat.read()).split())

assignment.remove('0')

if 'UNSATISFIABLE' in assignment:
    print('UNSATISFIABLE')
    exit(0)

ok = True
clsok = False
with open(sys.argv[1]) as cnf:
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

if not ok:
    print(assignment)
    print(clause)
    print('WA')
    exit(0)
else:
    print('AC')
