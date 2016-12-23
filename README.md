# YA-SAT
Yet Another SAT Solver. Term project of PACA.


## Tasks

### TODO
- Iterative BCP
- Marked Learnt Clause
- Restart strategy

### DONE
- 2 Literal Watching
- Iterative DPLL
- Conflict Clause Learning
- Nonchronological Backtracking
- Learnt Clause Minimization
- Split independent subproblems
- Decision BCP result cache
- High performance watcher check list
- VSIDS


## How to
- To build `./build.sh`  
- To run `./yasat [options] benchmark.cnf`  
    - `-statistic` print statistic result to _stderr_  
    - `-stdout` print result to stdout instead of file  
    - `-no` don't use any heuristic  
    - `-mom` (default) use MOM branching heuristic  
    - `-jw` use Jeroslaw-Wang branchgin heuristic
- To run all benchmarks in *.cnf under directory `python3 test.py path-to-directory`  
- To validate the sat result `python3 validator.py benchmark.cnf benchmark.sat`  
- To run all SAT benchmarks  
    1. put all SAT benchmarks under `benchmarks/SAT/`  
    2. run `python3 testsat.py`  
- To run all UNSAT benchmarks  
    1. put all UNSAT benchmarks under `benchmarks/UNSAT/`
    2. run `python3 testunsat.py`

## Result
Beside testcases provied by PACA, I also test online benchmarks from [SATLIB](http://www.cs.ubc.ca/~hoos/SATLIB/benchm.html).  
`sat-100-430` means 100 variables and 430 clauses all satisfiable.  
Run-times obtained on Intel Pentium CPU N3540 @ 2.16GHz × 4

### Elapsed time
Only time took by SAT solver is counted, excluding parsing time, init time.  

| Scale          |          basic |          vsids |    minisat |
| -------------- | -------------: | -------------: | ---------: |
| sat-20-91      |      0.00/0.00 |      0.00/0.00 |  0.00/0.01 |
| sat-50-218     |      0.00/0.00 |      0.00/0.00 |  0.00/0.01 |
| sat-75-325     |      0.00/0.01 |      0.00/0.01 |  0.00/0.01 |
| sat-100-430    |      0.00/0.02 |      0.00/0.02 |  0.01/0.02 |
| sat-125-538    |      0.02/0.07 |      0.02/0.08 |  0.01/0.03 |
| sat-150-645    |      0.05/0.34 |      0.04/0.33 |  0.02/0.06 |
| sat-250-1065   |   39.24/367.79 |   42.31/721.33 | 1.60/16.19 |
| unsat-50-218   |      0.00/0.00 |      0.00/0.00 |  0.00/0.01 |
| unsat-75-325   |      0.00/0.01 |      0.00/0.01 |  0.00/0.01 |
| unsat-100-430  |      0.01/0.04 |      0.01/0.03 |  0.01/0.02 |
| unsat-125-538  |      0.04/0.13 |      0.04/0.11 |  0.01/0.03 |
| unsat-150-645  |      0.17/0.74 |      0.14/0.34 |  0.04/0.08 |
| unsat-250-1065 | 207.99/1184.38 | 279.10/1149.57 | 7.41/19.70 |


### Backtrack num
| Scale          |                basic |               vsids |
| -------------- | -------------------: | ------------------: |
| sat-20-91      |           2.45/13.00 |          2.50/14.00 |
| sat-50-218     |          14.32/92.00 |         14.60/98.00 |
| sat-75-325     |         51.79/221.00 |        62.75/272.00 |
| sat-100-430    |        156.33/869.00 |      163.08/1031.00 |
| sat-125-538    |       550.24/2555.00 |      654.62/3148.00 |
| sat-150-645    |      1395.59/7145.00 |     1307.87/9115.00 |
| sat-250-1065   |  104605.31/550331.00 |  78843.48/550100.00 |
| unsat-50-218   |         44.99/119.00 |        46.45/130.00 |
| unsat-75-325   |        148.66/361.00 |       159.84/326.00 |
| unsat-100-430  |       489.88/1351.00 |      537.46/1621.00 |
| unsat-125-538  |      1504.70/4004.00 |     1657.05/4401.00 |
| unsat-150-645  |     4265.77/12745.00 |     4712.63/8717.00 |
| unsat-250-1065 | 368849.21/1214901.00 | 312821.13/730662.00 |


### Search Tree Depth
| Scale          |       basic |       vsids |
| -------------- | ----------: | ----------: |
| sat-20-91      |  6.41/16.00 |  6.38/15.00 |
| sat-50-218     | 11.79/26.00 | 11.66/27.00 |
| sat-75-325     | 15.56/31.00 | 15.86/30.00 |
| sat-100-430    | 20.18/44.00 | 19.57/38.00 |
| sat-125-538    | 23.79/39.00 | 22.45/39.00 |
| sat-150-645    | 27.48/51.00 | 26.90/45.00 |
| sat-250-1065   | 42.63/68.00 | 39.13/62.00 |
| unsat-50-218   |  7.65/14.00 |  7.79/17.00 |
| unsat-75-325   | 10.78/16.00 | 10.96/17.00 |
| unsat-100-430  | 13.80/21.00 | 14.45/24.00 |
| unsat-125-538  | 16.92/22.00 | 17.55/24.00 |
| unsat-150-645  | 19.61/26.00 | 20.68/27.00 |
| unsat-250-1065 | 30.55/37.00 | 31.41/38.00 |













