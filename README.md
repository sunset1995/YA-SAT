# YA-SAT
Yet Another SAT Solver. Term project of PACA.


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
Beside testcases provied by PACA, I also use online benchmark problems from [SATLIB](http://www.cs.ubc.ca/~hoos/SATLIB/benchm.html).  
`sat/100-430` means 100 variables and 430 clauses.  
I implement _two literal watching_ before recursive. So even the simplest version has _two literal watching_ and _Unit Clause Rule_.  

### Elapsed time
Only time on SAT solver's _\_solve_ function is counted, excluding parsing time, init time.  

| Scale | # instances | No(avg/max) | MOM | JW |
| ---- | ----: | ----: | ----: | ----: |
| sat/sanity | 2 | 0/0 | 0/0 | 0/0 |
| sat/tiny | 2 | 0/0 | 0/0 | 0/0 |
| sat/20-91 | 1000 | 0/0 | 0/0 | 0/0 |
| sat/50-218 | 1000 | 0/0 | 0/0 | 0/0.001 |
| sat/75-325 | 100 | 0.007/0.040 | 0/0.004 | 0.001/0.003 |
| sat/100-430 | 1000 | 0.075/1.208 | 0.002/0.016 | 0.004/0.019 |
| sat/125-538 | 100 | 0.975/20.521 | 0.014/0.103 | 0.018/0.087 |
| sat/150-645 | 100 | - | 0.041/0.254 | 0.058/0.403 |
| unsat/sanity | 2 | 0/0 | 0/0 | 0/0 |
| unsat/tiny | 2 | 0/0 | 0/0 | 0/0 |
| unsat/50-218 | 1000 | 0.001/0.008 | 0/0.001 | 0/0.001 |
| unsat/75-325 | 100 | 0.015/0.051 | 0.006/0.029 | 0.002/0.004 |
| unsat/100-430 | 1000 | 0.208/2.443 | 0.001/0.004 | 0.008/0.037 |
| unsat/125-538 | 100 | 2.294/12.759 | 0.029/0.096 | 0.036/0.137 |
| unsat/150-645 | 100 | - | 0.144/0.659 | 0.152/0.774 |

### Backtrack num
| Scale | No(avg/max) | MOM | JW |
| ---- | ----: | ----: | ----: |
| sat/sanity | 0/0 | 0/0 | 0/0 |
| sat/tiny | 0/0 | 0/0 | 0/0 |
| sat/20-91 | 6/46 | 1/16 | 2/15 |
| sat/50-218 | 184/1728 | 26/199 | 22/122 |
| sat/75-325 | 2162/10754 | 133/661 | 109/485 |
| sat/100-430 | 20742/352322 | 443/3861 | 422/2173 |
| sat/125-538 | 243397/5123539 | 2399/17278 | 1974/8635 |
| sat/150-645 | - | 6907/46221 | 5667/40963 |
| unsat/sanity | 0/0 | 0/0 | 0/0 |
| unsat/tiny | 6/8 | 3/4 | 3/4 |
| unsat/50-218 | 478/3860 | 70/308 | 59/177 |
| unsat/75-325 | 4719/17045 | 306/1316 | 237/520 |
| unsat/100-430 | 60768/848845 | 1388/7757 | 997/3096 |
| unsat/125-538 | 534588/2672851 | 5761/21496 | 4151/13269 |
| unsat/150-645 | - | 25638/117297 | 15663/84435 |

### Search Tree Depth
| Scale | No(avg/max) | MOM | JW |
| ---- | ----: | ----: | ----: |
| sat/sanity | 3/5 | 4/6 | 2/3 |
| sat/tiny | 5/6 | 7/10 | 6/8 |
| sat/20-91 | 8/14 | 1/17 | 6/16 |
| sat/50-218 | 16/29 | 13/26 | 13/24 |
| sat/75-325 | 23/33 | 18/28 | 18/28 |
| sat/100-430 | 30/44 | 25/44 | 22/37 |
| sat/125-538 | 37/49 | 30/45 | 27/40 |
| sat/150-645 | - | 35/58 | 32/47 |
| unsat/sanity | 0/0 | 0/0 | 0/0 |
| unsat/tiny | 3/4 | 2/3 | 2/3 |
| unsat/50-218 | 15/25 | 10/16 | 9/15 |
| unsat/75-325 | 22/33 | 14/23 | 13/19 |
| unsat/100-430 | 30/43 | 19/30 | 18/26 |
| unsat/125-538 | 37/50 | 24/32 | 23/28 |
| unsat/150-645 | - | 28/35 | 27/33 |
