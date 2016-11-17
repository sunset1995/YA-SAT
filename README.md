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


## Result
Beside testcases provied by PACA, I also use online benchmark problems from [SATLIB](http://www.cs.ubc.ca/~hoos/SATLIB/benchm.html).  
`sat/100-430` means 100 variables and 430 clauses.  
I implement _two literal watching_ before recursive. So even the simplest version has _two literal watching_ and _Unit Clause Rule_.  

### Elapsed time
Only time on SAT solver's _\_solve_ function is counted, excluding parsing time, init time.  

| Scale | # instances | No(avg/max) |
| ---- | ----: | ----: |
| sat/sanity | 2 | 0/0 |
| unsat/sanity | 2 | 0/0 |
| sat/tiny | 2 | 0/0 |
| unsat/tiny | 2 | 0/0 |
| sat/20-91 | 1000 | 0/0 |
| sat/50-218 | 1000 | 0/0 |
| unsat/50-218 | 1000 | 0.001/0.008 |
| sat/75-325 | 100 | 0.007/0.040 |
| unsat/75-325 | 100 | 0.015/0.051 |
| sat/100-430 | 1000 | 0.075/1.208 |
| unsat/100-430 | 1000 | 0.208/2.443 |
| sat/125-538 | 100 | 0.975/20.521 |
| unsat/125-538 | 100 | 2.294/12.759 |

### Backtrack num
| Scale | No(avg/max) |
| ---- | ----: |
| sat/sanity | 0/0 | 
| unsat/sanity | 0/0 |
| sat/tiny | 0/0 |
| unsat/tiny | 6/8 |
| sat/20-91 | 6/46 |
| sat/50-218 | 184/1728 |
| unsat/50-218 | 478/3860 |
| sat/75-325 | 2162/10754 |
| unsat/75-325 | 4719/17045 |
| sat/100-430 | 20742/352322 |
| unsat/100-430 | 60768/848845 |
| sat/125-538 | 243397/5123539 |
| unsat/125-538 | 534588/2672851 |

### Search Tree Depth
| Scale | No(avg/max) |
| ---- | ----: |
| sat/sanity | 3/5 | 
| unsat/sanity | 0/0 |
| sat/tiny | 5/6 |
| unsat/tiny | 3/4 |
| sat/20-91 | 8/14 |
| sat/50-218 | 16/29 |
| unsat/50-218 | 15/25 |
| sat/75-325 | 23/33 |
| unsat/75-325 | 22/33 |
| sat/100-430 | 30/44 |
| unsat/100-430 | 30/43 |
| sat/125-538 | 37/49 |
| unsat/125-538 | 37/50 |
