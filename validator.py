import os
import sys

assignment = set()

with open(sys.argv[2]) as sat:
    assignment = set(str(sat.read()).split())

assignment.remove('0')

if 'UNSATISFIABLE' in assignment:
    print('UNSATISFIABLE')
    exit(0)

with open(sys.argv[1]) as cnf:
    for line in cnf:
        clause = line.split()
        if clause[0] == 'p' or clause[0] == 'c':
            continue
        ok = False
        for v in clause:
            if v in assignment:
                ok = True
                break
        if not ok:
            print(assignment)
            print(clause)
            print('WA')
            exit(0)

print('AC')
