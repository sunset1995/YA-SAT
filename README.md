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
- ANRFA


## How to
- To build `./build.sh`  
- To run `./yasat [options] benchmark.cnf`  
	- `-statistic` print statitic result to stderr
    - `-stdout   ` print result to stdout instead of file
    - `-nomom    ` disable init var score by MOM
    - `-noruby   ` disable ruby sequence to restart
    - `-stupid   ` restart rapidly stupidly
    - `-novsids  ` disable Variable State Independent Decaying Sum heuristic
    - `-nophase  ` disable phase saving
    - `-noanrfa  ` disable Average Number Recently Flipped Assignments
    - `-multi    ` enable multi-thread running all method concurrently
    - `-rand     ` add random factor in VSISD
    - `-pos      ` pick positive phase while tie
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
TLE(time limit exceed) is set to 1200 seconds.  

### Benchmark from course PACA  
For those whose correct answer is SAT, the answer is checked by my validator.  

| M2 testcase              |      M2 |    5e3 |    704 |   416 |
| ------------------------ | ------: | -----: | -----: | ----: |
| aim-50-1_6-no-2.cnf      |   0.000 |  0.000 |  0.000 | 0.000 |
| aim-50-1_6-no-3.cnf      |   0.000 |  0.000 |  0.000 | 0.000 |
| aim-50-1_6-no-4.cnf      |   0.000 |  0.000 |  0.000 | 0.000 |
| aim-50-1_6-yes1-1.cnf    |   0.000 |  0.000 |  0.000 | 0.000 |
| aim-50-1_6-yes1-2.cnf    |   0.000 |  0.000 |  0.000 | 0.000 |
| aim-50-1_6-yes1-3.cnf    |   0.000 |  0.000 |  0.000 | 0.000 |
| aim-50-1_6-yes1-4.cnf    |   0.000 |  0.000 |  0.000 | 0.000 |
| dubois26.cnf             |   0.005 |  0.007 |  0.006 | 0.022 |
| dubois27.cnf             |   0.002 |  0.003 |  0.003 | 0.015 |
| dubois28.cnf             |   0.002 |  0.005 |  0.008 | 0.015 |
| dubois29.cnf             |   0.003 |  0.005 |  0.003 | 0.007 |
| hole6.cnf                |   0.019 |  0.013 |  0.015 | 0.012 |
| hole7.cnf                |   0.208 |  0.130 |  0.134 | 0.105 |
| hole8.cnf                |   5.229 |  3.191 |  1.370 | 0.822 |
| hole9.cnf                | 200.973 | 76.687 | 15.186 | 9.996 |
| ii8a1.cnf                |   0.000 |  0.000 |  0.000 | 0.000 |
| ii8a2.cnf                |   0.000 |  0.000 |  0.000 | 0.000 |
| ii8a3.cnf                |   0.001 |  0.001 |  0.001 | 0.001 |
| ii8a4.cnf                |   0.001 |  0.001 |  0.002 | 0.001 |
| par8-1-c.cnf             |   0.000 |  0.000 |  0.000 | 0.000 |
| par8-2-c.cnf             |   0.000 |  0.000 |  0.000 | 0.000 |
| par8-3-c.cnf             |   0.000 |  0.000 |  0.001 | 0.001 |
| par8-4-c.cnf             |   0.000 |  0.000 |  0.000 | 0.000 |
| aim-100-1_6-no-1.cnf     |   0.001 |  0.000 |  0.000 | 0.000 |
| aim-100-1_6-no-2.cnf     |   0.001 |  0.000 |  0.001 | 0.001 |
| aim-100-1_6-no-3.cnf     |   0.001 |  0.001 |  0.001 | 0.001 |
| aim-100-1_6-no-4.cnf     |   0.001 |  0.001 |  0.001 | 0.000 |
| aim-100-1_6-yes1-1.cnf   |   0.000 |  0.000 |  0.000 | 0.000 |
| aim-100-1_6-yes1-2.cnf   |   0.000 |  0.000 |  0.001 | 0.001 |
| aim-100-1_6-yes1-3.cnf   |   0.001 |  0.000 |  0.001 | 0.001 |
| aim-100-1_6-yes1-4.cnf   |   0.001 |  0.000 |  0.000 | 0.000 |
| aim-200-1_6-no-1.cnf     |   0.001 |  0.001 |  0.001 | 0.001 |
| aim-200-1_6-no-2.cnf     |   0.001 |  0.001 |  0.001 | 0.001 |
| aim-200-1_6-no-3.cnf     |   0.002 |  0.001 |  0.002 | 0.001 |
| aim-200-1_6-no-4.cnf     |   0.001 |  0.001 |  0.001 | 0.000 |
| aim-200-1_6-yes1-1.cnf   |   0.001 |  0.001 |  0.001 | 0.001 |
| aim-200-1_6-yes1-2.cnf   |   0.003 |  0.002 |  0.002 | 0.002 |
| aim-200-1_6-yes1-3.cnf   |   0.001 |  0.001 |  0.001 | 0.001 |
| aim-200-1_6-yes1-4.cnf   |   0.002 |  0.001 |  0.001 | 0.001 |
| jnh201.cnf               |   0.001 |  0.001 |  0.001 | 0.001 |
| jnh210.cnf               |   0.000 |  0.001 |  0.001 | 0.001 |
| jnh301.cnf               |   0.004 |  0.003 |  0.003 | 0.004 |
| jnh310.cnf               |   0.000 |  0.000 |  0.000 | 0.000 |
| ii16a1.cnf               |   0.010 |  0.144 |  0.029 | 0.042 |
| ii16a2.cnf               |   0.010 |  0.292 |  2.187 | 0.730 |
| ii16b1.cnf               |   0.026 |  1.662 |  1.987 | 0.211 |
| ii16b2.cnf               |   0.043 |  0.030 |  0.016 | 0.066 |
| ii32b1.cnf               |   0.001 |  0.001 |  0.001 | 0.001 |
| ii32b2.cnf               |   0.008 |  0.013 |  0.023 | 0.028 |
| ii32b3.cnf               |   0.011 |  0.033 |  0.183 | 0.035 |
| ii32b4.cnf               |   0.021 |  0.106 |  0.387 | 0.096 |
| par16-1-c.cnf            |   0.054 |  0.178 |  0.087 | 0.094 |
| par16-2-c.cnf            |   0.274 |  0.203 |  0.347 | 0.235 |
| par16-3-c.cnf            |   0.538 |  0.402 |  0.671 | 1.068 |
| par16-4-c.cnf            |   0.042 |  0.162 |  0.181 | 0.150 |


Beside testcases provied by PACA, I also test online benchmarks from [SATLIB](http://www.cs.ubc.ca/~hoos/SATLIB/benchm.html).  
`sat-100-430` means 100 variables and 430 clauses all satisfiable. Result record in `avg/max` format.   

| Scale          |         5e3 |          704 |          416 |
| -------------- | ----------: | -----------: | -----------: |
| sat-20-91      |   0.00/0.00 |    0.00/0.00 |    0.00/0.00 |
| sat-50-218     |   0.00/0.00 |    0.00/0.00 |    0.00/0.00 |
| sat-75-325     |   0.00/0.01 |    0.00/0.01 |    0.00/0.01 |
| sat-100-430    |   0.00/0.02 |    0.00/0.02 |    0.00/0.02 |
| sat-125-538    |   0.01/0.06 |    0.01/0.05 |    0.01/0.04 |
| sat-150-645    |   0.03/0.14 |    0.03/0.16 |    0.03/0.15 |
| sat-250-1065   |  3.89/43.49 |   4.47/24.05 |   5.70/33.84 |
| unsat-50-218   |   0.00/0.00 |    0.00/0.00 |    0.00/0.00 |
| unsat-75-325   |   0.00/0.01 |    0.00/0.01 |    0.00/0.01 |
| unsat-100-430  |   0.01/0.02 |    0.01/0.03 |    0.01/0.02 |
| unsat-125-538  |   0.03/0.06 |    0.03/0.06 |    0.03/0.06 |
| unsat-150-645  |   0.08/0.17 |    0.08/0.17 |    0.08/0.21 |
| unsat-250-1065 | 23.02/79.67 | 19.95/114.28 | 29.16/111.99 |


Also I run SAT competition 2006’s benchmark_1 and compare the result with those competitor.  

|               instance    |  ans  | compsat | HaifaSat | Jerusat1_2_c | minisat_stat | sat4j | SatELiteGTI | zchaff-2004 |    yasat |
| ------------------------- | ----- | ------: | -------: | -----------: | -----------: | ----: | ----------: | ----------: | -------: |
|    een-tipb-sr06-par1.cnf | UNSAT |    15.1 |     35.7 |         11.0 |          1.6 |   6.5 |         2.9 |         2.2 |    1.201 |
|    een-tipb-sr06-tc6b.cnf | UNSAT |     2.4 |     52.8 |          2.0 |          0.3 |   1.7 |         1.0 |         0.5 |    0.509 |
|     goldb-heqc-desmul.cnf | UNSAT |   372.6 |    124.6 |            X |         78.8 | 217.6 |        77.0 |       157.3 |  162.977 |
|     goldb-heqc-rotmul.cnf | UNSAT |       X |    306.3 |            X |        165.0 |     X |       257.3 |       136.9 |  456.686 |
|    grieu-vmpc-s05-24s.cnf |  SAT  |       X |    371.6 |        210.7 |         13.8 | 341.3 |        13.9 |           X |   81.745 |
|    grieu-vmpc-s05-27r.cnf |  SAT  |       X |        X |         17.6 |        701.1 |     X |       691.8 |       462.9 |        X |
|     hoons-vbmc-s04-05.cnf | UNSAT |       X |    106.3 |            X |        251.1 | 128.0 |        20.5 |       316.4 |  238.522 |
|     hoons-vbmc-s04-07.cnf | UNSAT |       X |        X |        597.0 |        194.9 |     X |        53.3 |           X |  277.209 |
|       manol-pipe-c10b.cnf | UNSAT |       X |        X |            X |        438.4 |     X |       618.2 |       618.4 |  375.224 |
|    manol-pipe-c10ni_s.cnf | UNSAT |   130.6 |    124.7 |         40.8 |         19.6 |  70.4 |         8.5 |           X |  439.861 |
|       manol-pipe-c6id.cnf | UNSAT |       X |    205.4 |            X |        156.1 | 364.7 |        68.4 |       480.1 |   36.683 |
|        manol-pipe-c6n.cnf | UNSAT |   362.4 |    124.4 |            X |         53.7 | 230.0 |        34.6 |       110.2 |   82.242 |
|    manol-pipe-c6nid_s.cnf | UNSAT |   125.2 |     31.5 |            X |         96.5 |  66.8 |        57.6 |       663.0 |        X |
|       manol-pipe-c7_i.cnf | UNSAT |   243.5 |     43.8 |            X |         17.7 |  62.8 |         9.4 |         7.1 |  120.825 |
|      manol-pipe-c7idw.cnf | UNSAT |       X |    357.6 |            X |        483.7 | 713.0 |       566.5 |       865.0 | 1123.455 |
|       manol-pipe-c8_i.cnf | UNSAT |   279.4 |     45.6 |            X |         18.1 |  84.3 |        35.6 |         8.8 |        X |
|      manol-pipe-c8b_i.cnf | UNSAT |       X |    544.2 |            X |        146.1 | 591.0 |        94.6 |       227.3 |        X |
|        manol-pipe-c8n.cnf | UNSAT |       X |    638.1 |            X |        153.9 | 777.6 |        72.8 |       353.7 |   61.918 |
|        manol-pipe-f6b.cnf | UNSAT |    25.0 |     30.7 |        111.5 |          4.1 |  25.9 |         2.8 |         5.8 |   29.550 |
|        manol-pipe-f6n.cnf | UNSAT |    28.8 |     33.5 |        168.4 |          8.8 |  20.3 |         3.0 |         6.9 |   24.757 |
|     manol-pipe-g10idw.cnf | UNSAT |       X |    237.4 |            X |        244.0 |     X |       482.7 |       541.6 |   15.363 |
|        manol-pipe-g7n.cnf | UNSAT |    70.7 |     31.9 |        685.9 |          4.2 |  20.8 |         2.1 |        10.0 |   19.465 |
|      manol-pipe-g6bid.cnf | UNSAT |    29.8 |     15.0 |        715.5 |          7.7 |  17.4 |         2.5 |        16.6 |        X |
|         narai-vpn-10s.cnf | UNSAT |       X |        X |            X |        681.2 |     X |       866.7 |       605.4 |  412.016 |
|    schup-l2s-s04-abp4.cnf | UNSAT |       X |    181.4 |            X |        170.9 |     X |       122.7 |           X |  708.045 |
| simon-mixed-s02bis-01.cnf |  SAT  |       X |        X |            X |        215.2 | 186.2 |       607.0 |           X | 1042.069 |
| simon-mixed-s02bis-03.cnf |  SAT  |       X |     30.5 |            X |        236.4 |     X |       164.2 |           X |  193.690 |
| simon-mixed-s02bis-05.cnf |  SAT  |       X |        X |            X |            X |     X |       577.9 |           X |        X |
|      stric-bmc-ibm-10.cnf |  SAT  |    32.1 |     25.0 |          3.6 |          2.9 |   9.7 |         2.3 |        11.1 |    7.140 |
|      stric-bmc-ibm-12.cnf |  SAT  |    43.3 |     70.0 |         51.5 |          8.8 |  19.6 |         6.3 |        29.5 |   51.332 |
|    vange-color-inc-54.cnf |  SAT  |    89.2 |    151.2 |         80.5 |         28.5 |  72.6 |         8.1 |           X |   14.600 |
|  velev-eng-uns-1.0-04.cnf | UNSAT |    31.6 |     19.7 |         76.3 |         10.0 |  24.5 |         8.7 |        30.1 |   50.747 |
| velev-eng-uns-1.0-04a.cnf | UNSAT |   215.9 |    171.6 |        358.2 |         55.1 | 119.9 |        49.1 |       549.1 |  133.157 |
|  velev-fvp-sat-3.0-07.cnf |  SAT  |   396.2 |    114.8 |         57.3 |            X | 346.0 |           X |       890.7 |   30.603 |
|  velev-fvp-sat-3.0-12.cnf |  SAT  |    71.0 |     50.2 |         57.0 |          6.3 |  25.4 |         3.8 |         3.0 |   25.085 |
| velev-live-sat-1.0-03.cnf |  SAT  |   131.0 |        X |            X |         10.2 |  55.8 |       124.7 |           X |  345.989 |
| velev-live-sat-1.0-01.cnf |  SAT  |   191.9 |    209.7 |            X |         13.4 |  28.8 |        23.8 |       626.6 |  443.933 |
|      velev-npe-1.0-02.cnf | UNSAT |       X |     24.5 |        346.3 |         16.2 | 164.9 |        11.8 |        59.0 |  137.459 |
|      velev-npe-1.0-03.cnf |  SAT  |   220.3 |     77.3 |            X |          5.9 | 359.4 |        35.6 |       327.0 |  149.678 |
|     velev-pipe-1.0-08.cnf |  SAT  |    12.9 |     67.6 |        221.7 |            X |  43.2 |           X |        66.6 |  760.339 |
|     velev-pipe-1.0-09.cnf |  SAT  |   683.4 |    288.3 |            X |          7.1 | 706.3 |       145.7 |           X |        X |
|     velev-pipe-1.1-03.cnf |  SAT  |   789.5 |    404.3 |            X |          7.1 |     X |        29.7 |           X |        X |
|     velev-pipe-1.1-05.cnf |  SAT  |   356.2 |    418.6 |            X |          4.9 | 339.2 |        29.8 |       202.6 |   99.089 |
| velev-pipe-uns-1.0-08.cnf | UNSAT |       X |    638.0 |            X |            X |     X |           X |       843.3 |        X |
| velev-pipe-uns-1.1-05.cnf | UNSAT |   345.3 |     14.5 |            X |            X | 389.6 |           X |        17.7 |        X |
| velev-pipe-uns-1.1-07.cnf | UNSAT |       X |    161.0 |            X |            X |     X |           X |       195.5 |        X |
|      velev-sss-1.0-cl.cnf | UNSAT |     0.9 |      0.5 |         61.1 |          0.4 |   1.6 |         0.3 |         0.4 |    2.765 |
| velev-vliw-sat-2.0-02.cnf |  SAT  |    39.1 |        X |            X |         13.6 |  36.5 |        55.1 |        22.4 |  221.719 |
| velev-vliw-sat-2.0-04.cnf |  SAT  |       X |        X |            X |            X |     X |       139.4 |           X |        X |
| velev-vliw-uns-2.0-02.cnf | UNSAT |       X |        X |            X |            X |     X |           X |           X |        X |
