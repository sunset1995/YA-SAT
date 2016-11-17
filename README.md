# YA-SAT
Yet Another SAT Solver. Term project of PACA.


## How to
- To build `make clean && make`  
- To run `./yasat [options] benchmark.cnf`  
    - `-statistic` print statistic result to _stderr_  
    - `-stdout` print result to stdout instead of file  
    - `-no` don't use any heuristic  
    - `-mom` (default) use MOM branching heuristic  
- To test all benchmarks, *.cnf, under directory `python3 test.py path`  
- To validate the sat result `python3 validator.py benchmark.cnf benchmark.sat`  


## Result
Beside testcases provied by PACA, I also use online benchmark problems from [SATLIB](http://www.cs.ubc.ca/~hoos/SATLIB/benchm.html).  
`sat/100-430` means 100 variables and 430 clauses.  
I implement _two literal watching_ before recursive. So even the simplest version has _two literal watching_ and _Unit Clause Rule_.  

### Elapsed time
Only time on SAT solver's _\_solve_ function is counted, excluding parsing time, init time.  

| Scale | # instances | No(avg/max) | MOM |
| ---- | ----: | ----: | ----: |
| sat/sanity | 2 | 0/0 | 0/0 |
| unsat/sanity | 2 | 0/0 | 0/0 |
| sat/tiny | 2 | 0/0 | 0/0 |
| unsat/tiny | 2 | 0/0 | 0/0 |
| sat/20-91 | 1000 | 0/0 | 0/0 |
| sat/50-218 | 1000 | 0/0 | 0/0 |
| unsat/50-218 | 1000 | 0.001/0.008 | 0/0.001 |
| sat/75-325 | 100 | 0.007/0.040 | 0/0.004 |
| unsat/75-325 | 100 | 0.015/0.051 | 0.006/0.029 |
| sat/100-430 | 1000 | 0.075/1.208 | 0.003/0.034 |
| unsat/100-430 | 1000 | 0.208/2.443 | 0.001/0.004 |
| sat/125-538 | 100 | 0.975/20.521 | 0.015/0.074 |
| unsat/125-538 | 100 | 2.294/12.759 | 0.029/0.096 |
| sat/150-645 | 100 | - | 0.048/0.254 |
| unsat/150-645 | 100 | - | 0.144/0.659 |

### Backtrack num
| Scale | No(avg/max) | MOM |
| ---- | ----: | ----: |
| sat/sanity | 0/0 | 0/0 |
| unsat/sanity | 0/0 | 0/0 |
| sat/tiny | 0/0 | 0/0 |
| unsat/tiny | 6/8 | 3/4 |
| sat/20-91 | 6/46 | 2/16 |
| sat/50-218 | 184/1728 | 26/199 |
| unsat/50-218 | 478/3860 | 70/308 |
| sat/75-325 | 2162/10754 | 133/661 |
| unsat/75-325 | 4719/17045 | 306/1316 |
| sat/100-430 | 20742/352322 | 584/4215 |
| unsat/100-430 | 60768/848845 | 1388/7757 |
| sat/125-538 | 243397/5123539 | 2800/14917 |
| unsat/125-538 | 534588/2672851 | 5761/21496 |
| sat/150-645 | - | 8456/46206 |
| unsat/150-645 | - | 25638/117297 |

### Search Tree Depth
| Scale | No(avg/max) | MOM |
| ---- | ----: | ----: |
| sat/sanity | 3/5 | 3/5 |
| unsat/sanity | 0/0 | 0/0 |
| sat/tiny | 5/6 | 3/4 |
| unsat/tiny | 3/4 | 2/3 |
| sat/20-91 | 8/14 | 7/14 |
| sat/50-218 | 16/29 | 13/26 |
| unsat/50-218 | 15/25 | 10/16 |
| sat/75-325 | 23/33 | 18/28 |
| unsat/75-325 | 22/33 | 14/23 |
| sat/100-430 | 30/44 | 23/46 |
| unsat/100-430 | 30/43 | 19/30 |
| sat/125-538 | 37/49 | 29/38 |
| unsat/125-538 | 37/50 | 24/32 |
| sat/150-645 | - | 33/47 |
| unsat/150-645 | - | 28/35 |
