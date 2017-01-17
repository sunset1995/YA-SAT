# YA-SAT
Yet Another SAT Solver. Term project of PACA.


## Tasks

### TODO
- Iterative BCP
- Marked Learnt Clause
- Memory usage
- More preprocess

### DONE
- 2 Literal Watching
- Iterative DPLL
- Conflict Clause Learning
- Nonchronological Backtracking
- Learnt Clause Minimization
- Split independent subproblems
- Decision BCP result cache
- Circular list watcher check list
- VSIDS
- Phase saving


## How to
- To build `./build.sh`  
- To run `./yasat [options] benchmark.cnf`  
    - `-statistic` print statistic result to _stderr_  
    - `-stdout` print result to stdout instead of file  
    - `-no` don't use any heuristic  
    - `-mom` (default) use MOM branching heuristi
- To run all benchmarks in *.cnf under directory `python3 test.py path-to-directory`  
- To validate the sat result `python3 validator.py benchmark.cnf benchmark.sat`  
- To run all SAT benchmarks  
    1. put all SAT benchmarks under `benchmarks/SAT/`  
    2. run `python3 testsat.py`  
- To run all UNSAT benchmarks  
    1. put all UNSAT benchmarks under `benchmarks/UNSAT/`
    2. run `python3 testunsat.py`

## Result
Run-times obtained on Intel Pentium CPU N3540 @ 2.16GHz × 4 and recored in second.  
TLE(time limit exceed) set to 1200 seconds.  

### Milestone-2 benchmark
Run-time is compared with _minisat_  
For those whose correct answer is SAT, the answer is checked by my validator.  

| M2 testcase              |    5e3 |    704 |
| ------------------------ | -----: | -----: |
| aim-50-1_6-no-2.cnf      |  0.000 |  0.000 |
| aim-50-1_6-no-3.cnf      |  0.000 |  0.000 |
| aim-50-1_6-no-4.cnf      |  0.000 |  0.000 |
| aim-50-1_6-yes1-1.cnf    |  0.000 |  0.000 |
| aim-50-1_6-yes1-2.cnf    |  0.000 |  0.000 |
| aim-50-1_6-yes1-3.cnf    |  0.000 |  0.000 |
| aim-50-1_6-yes1-4.cnf    |  0.000 |  0.000 |
| dubois26.cnf             |  0.007 |  0.006 |
| dubois27.cnf             |  0.003 |  0.003 |
| dubois28.cnf             |  0.005 |  0.008 |
| dubois29.cnf             |  0.005 |  0.003 |
| hole6.cnf                |  0.013 |  0.015 |
| hole7.cnf                |  0.130 |  0.134 |
| hole8.cnf                |  3.191 |  1.370 |
| hole9.cnf                | 76.687 | 15.186 |
| ii8a1.cnf                |  0.000 |  0.000 |
| ii8a2.cnf                |  0.000 |  0.000 |
| ii8a3.cnf                |  0.001 |  0.001 |
| ii8a4.cnf                |  0.001 |  0.002 |
| par8-1-c.cnf             |  0.000 |  0.000 |
| par8-2-c.cnf             |  0.000 |  0.000 |
| par8-3-c.cnf             |  0.000 |  0.001 |
| par8-4-c.cnf             |  0.000 |  0.000 |
| aim-100-1_6-no-1.cnf     |  0.000 |  0.000 |
| aim-100-1_6-no-2.cnf     |  0.000 |  0.001 |
| aim-100-1_6-no-3.cnf     |  0.001 |  0.001 |
| aim-100-1_6-no-4.cnf     |  0.001 |  0.001 |
| aim-100-1_6-yes1-1.cnf   |  0.000 |  0.000 |
| aim-100-1_6-yes1-2.cnf   |  0.000 |  0.001 |
| aim-100-1_6-yes1-3.cnf   |  0.000 |  0.001 |
| aim-100-1_6-yes1-4.cnf   |  0.000 |  0.000 |
| aim-200-1_6-no-1.cnf     |  0.001 |  0.001 |
| aim-200-1_6-no-2.cnf     |  0.001 |  0.001 |
| aim-200-1_6-no-3.cnf     |  0.001 |  0.002 |
| aim-200-1_6-no-4.cnf     |  0.001 |  0.001 |
| aim-200-1_6-yes1-1.cnf   |  0.001 |  0.001 |
| aim-200-1_6-yes1-2.cnf   |  0.002 |  0.002 |
| aim-200-1_6-yes1-3.cnf   |  0.001 |  0.001 |
| aim-200-1_6-yes1-4.cnf   |  0.001 |  0.001 |
| jnh201.cnf               |  0.001 |  0.001 |
| jnh210.cnf               |  0.001 |  0.001 |
| jnh301.cnf               |  0.003 |  0.003 |
| jnh310.cnf               |  0.000 |  0.000 |
| ii16a1.cnf               |  0.144 |  0.029 |
| ii16a2.cnf               |  0.292 |  2.187 |
| ii16b1.cnf               |  1.662 |  1.987 |
| ii16b2.cnf               |  0.030 |  0.016 |
| ii32b1.cnf               |  0.001 |  0.001 |
| ii32b2.cnf               |  0.013 |  0.023 |
| ii32b3.cnf               |  0.033 |  0.183 |
| ii32b4.cnf               |  0.106 |  0.387 |
| par16-1-c.cnf            |  0.178 |  0.087 |
| par16-2-c.cnf            |  0.203 |  0.347 |
| par16-3-c.cnf            |  0.402 |  0.671 |
| par16-4-c.cnf            |  0.162 |  0.181 |


Beside testcases provied by PACA, I also test online benchmarks from [SATLIB](http://www.cs.ubc.ca/~hoos/SATLIB/benchm.html).  
`sat-100-430` means 100 variables and 430 clauses all satisfiable.  

### Elapsed time
Only time took by SAT solver is counted, excluding parsing time, init time.  

| Scale          |         5e3 |          704 |
| -------------- | ----------: | -----------: |
| sat-20-91      |   0.00/0.00 |    0.00/0.00 |
| sat-50-218     |   0.00/0.00 |    0.00/0.00 |
| sat-75-325     |   0.00/0.01 |    0.00/0.01 |
| sat-100-430    |   0.00/0.02 |    0.00/0.02 |
| sat-125-538    |   0.01/0.06 |    0.01/0.05 |
| sat-150-645    |   0.03/0.14 |    0.03/0.16 |
| sat-250-1065   |  3.89/43.49 |   4.47/24.05 |
| unsat-50-218   |   0.00/0.00 |    0.00/0.00 |
| unsat-75-325   |   0.00/0.01 |    0.00/0.01 |
| unsat-100-430  |   0.01/0.02 |    0.01/0.03 |
| unsat-125-538  |   0.03/0.06 |    0.03/0.06 |
| unsat-150-645  |   0.08/0.17 |    0.08/0.17 |
| unsat-250-1065 | 23.02/79.67 | 19.95/114.28 |
