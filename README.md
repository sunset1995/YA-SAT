# YA-SAT
Yet Another SAT Solver. Term project of PACA.


## Tasks

### TODO
- High performance watcher check list
- Iterative BCP
- Marked Learnt Clause

### DONE
- 2 Literal Watching
- Iterative DPLL
- Conflict Clause Learning
- Nonchronological Backtracking
- Learnt Clause Minimization
- Decision BCP result cache


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

| Scale          |          basic |
| -------------- | -------------: |
| sat-20-91      |      0.00/0.00 |
| sat-50-218     |      0.00/0.00 |
| sat-75-325     |      0.00/0.01 |
| sat-100-430    |      0.00/0.02 |
| sat-125-538    |      0.02/0.07 |
| sat-150-645    |      0.05/0.34 |
| sat-250-1065   |   39.24/367.79 |
| unsat-50-218   |      0.00/0.00 |
| unsat-75-325   |      0.00/0.01 |
| unsat-100-430  |      0.01/0.04 |
| unsat-125-538  |      0.04/0.13 |
| unsat-150-645  |      0.17/0.74 |
| unsat-250-1065 | 207.99/1184.38 |


### Backtrack num
| Scale          |                basic |
| -------------- | -------------------: |
| sat-20-91      |           2.45/13.00 |
| sat-50-218     |          14.32/92.00 |
| sat-75-325     |         51.79/221.00 |
| sat-100-430    |        156.33/869.00 |
| sat-125-538    |       550.24/2555.00 |
| sat-150-645    |      1395.59/7145.00 |
| sat-250-1065   |  104605.31/550331.00 |
| unsat-50-218   |         44.99/119.00 |
| unsat-75-325   |        148.66/361.00 |
| unsat-100-430  |       489.88/1351.00 |
| unsat-125-538  |      1504.70/4004.00 |
| unsat-150-645  |     4265.77/12745.00 |
| unsat-250-1065 | 368849.21/1214901.00 |


### Search Tree Depth
| Scale          |       basic |
| -------------- | ----------: |
| sat-20-91      |  6.41/16.00 |
| sat-50-218     | 11.79/26.00 |
| sat-75-325     | 15.56/31.00 |
| sat-100-430    | 20.18/44.00 |
| sat-125-538    | 23.79/39.00 |
| sat-150-645    | 27.48/51.00 |
| sat-250-1065   | 42.63/68.00 |
| unsat-50-218   |  7.65/14.00 |
| unsat-75-325   | 10.78/16.00 |
| unsat-100-430  | 13.80/21.00 |
| unsat-125-538  | 16.92/22.00 |
| unsat-150-645  | 19.61/26.00 |
| unsat-250-1065 | 30.55/37.00 |
