# Concurrent (Multithreaded) TCP-Server and Client (n = 1000)
```bash
 Performance counter stats for 'taskset -c 3-5 ./client 1000':

      61,82,74,775      cycles                                                      
      31,02,76,162      instructions              #    0.50  insn per cycle         
       4,70,23,528      cache-references                                            
       1,33,43,841      cache-misses              #   28.377 % of all cache refs    
             5,412      context-switches                                            
                 0      major-faults                                                
             2,385      minor-faults                                                
       7,56,21,131      branches                                                    

     112.360075971 seconds time elapsed

       0.009015000 seconds user
       0.169040000 seconds sys

```

```bash
 Performance counter stats for 'taskset -c 0-3 ./server m':

   18,59,16,79,277      cycles                                                      
   17,81,73,61,593      instructions              #    0.96  insn per cycle         
    1,62,77,05,744      cache-references                                            
      11,01,05,305      cache-misses              #    6.764 % of all cache refs    
             3,995      context-switches                                            
                 0      major-faults                                                
             4,424      minor-faults                                                
    4,09,84,43,830      branches                                                    

     113.530609496 seconds time elapsed

       0.556886000 seconds user
       3.609295000 seconds sys

```

# Concurrent (Multithreaded) TCP-Server and Client (n = 500)

```bash
 Performance counter stats for 'taskset -c 3-5 ./client 500':

      30,99,18,884      cycles                                                      
      15,62,11,116      instructions              #    0.50  insn per cycle         
       2,45,73,126      cache-references                                            
         74,24,828      cache-misses              #   30.215 % of all cache refs    
             2,188      context-switches                                            
                 0      major-faults                                                
             1,276      minor-faults                                                
       3,78,22,483      branches                                                    

       4.409081899 seconds time elapsed

       0.000000000 seconds user
       0.088274000 seconds sys
```

```bash
 Performance counter stats for 'taskset -c 0-2 ./server m':

    9,07,42,08,952      cycles                                                      
    8,76,29,39,919      instructions              #    0.97  insn per cycle         
      80,12,20,545      cache-references                                            
       5,21,55,052      cache-misses              #    6.509 % of all cache refs    
             1,961      context-switches                                            
                 0      major-faults                                                
             2,544      minor-faults                                                
    2,01,59,61,020      branches                                                    

       6.821300990 seconds time elapsed

       0.322856000 seconds user
       1.665271000 seconds sys

```

# Concurrent (Multithreaded) TCP-Server and Client (n = 100)

```bash
 Performance counter stats for 'taskset -c 3-5 ./client 100':

       6,16,37,679      cycles                                                      
       3,15,96,159      instructions              #    0.51  insn per cycle         
         53,62,333      cache-references                                            
         18,39,264      cache-misses              #   34.300 % of all cache refs    
               266      context-switches                                            
                 0      major-faults                                                
               385      minor-faults                                                
         75,84,490      branches                                                    

       1.077762481 seconds time elapsed

       0.000000000 seconds user
       0.017940000 seconds sys
```

```bash
 Performance counter stats for 'taskset -c 0-2 ./server m':

    1,83,26,06,003      cycles                                                      
    1,75,91,30,073      instructions              #    0.96  insn per cycle         
      16,00,55,730      cache-references                                            
       1,05,63,539      cache-misses              #    6.600 % of all cache refs    
               449      context-switches                                            
                 0      major-faults                                                
               560      minor-faults                                                
      40,48,07,307      branches                                                    

       2.536370480 seconds time elapsed

       0.070320000 seconds user
       0.339094000 seconds sys
```

# Concurrent (Multithreaded) TCP-Server and Client (n = 10)
```bash
 Performance counter stats for 'taskset -c 3-5 ./client 10':

       1,00,11,440      cycles                                                      
         54,62,024      instructions              #    0.55  insn per cycle         
          7,80,911      cache-references                                            
          2,87,876      cache-misses              #   36.864 % of all cache refs    
                32      context-switches                                            
                 0      major-faults                                                
               165      minor-faults                                                
         12,88,227      branches                                                    

       0.021800352 seconds time elapsed

       0.000000000 seconds user
       0.003235000 seconds sys
```

```bash
 Performance counter stats for 'taskset -c 0-2 ./server m':

      18,68,86,497      cycles                                                      
      17,63,22,583      instructions              #    0.94  insn per cycle         
       1,62,93,822      cache-references                                            
         10,60,311      cache-misses              #    6.507 % of all cache refs    
                10      context-switches                                            
                 0      major-faults                                                
               192      minor-faults                                                
       4,05,64,754      branches                                                    

       1.639824131 seconds time elapsed

       0.009072000 seconds user
       0.037735000 seconds sys

```

# Concurrent (Multithreaded) TCP-Server and Client (n = 2)
```bash
 Performance counter stats for 'taskset -c 3-5 ./client 2':

         41,69,859      cycles                                                      
         27,00,959      instructions              #    0.65  insn per cycle         
          3,01,958      cache-references                                            
          1,11,712      cache-misses              #   36.996 % of all cache refs    
                 7      context-switches                                            
                 0      major-faults                                                
               134      minor-faults                                                
          6,24,313      branches                                                    

       0.006771999 seconds time elapsed

       0.001949000 seconds user
       0.000000000 seconds sys

```


```bash
 Performance counter stats for 'taskset -c 0-2 ./server m':

       4,33,86,283      cycles                                                      
       3,74,94,046      instructions              #    0.86  insn per cycle         
         33,80,638      cache-references                                            
          2,68,493      cache-misses              #    7.942 % of all cache refs    
                10      context-switches                                            
                 0      major-faults                                                
               151      minor-faults                                                
         86,30,302      branches                                                    

       1.847221422 seconds time elapsed

       0.003754000 seconds user
       0.007509000 seconds sys

```

---

# Single Threaded TCP-Server and Multithreaded Client (n = 1000)

```bash
 Performance counter stats for 'taskset -c 3-5 ./client 1000':

      56,10,51,268      cycles                                                      
      29,08,91,987      instructions              #    0.52  insn per cycle         
       4,26,99,955      cache-references                                            
       1,10,62,897      cache-misses              #   25.908 % of all cache refs    
             6,127      context-switches                                            
                 0      major-faults                                                
             2,395      minor-faults                                                
       7,12,89,940      branches                                                    

     108.170299350 seconds time elapsed

       0.020979000 seconds user
       0.152284000 seconds sys

```

```bash
 Performance counter stats for 'taskset -c 0 ./server s':

   12,45,88,94,652      cycles                                                      
   12,45,18,53,148      instructions              #    1.00  insn per cycle         
    1,09,33,54,761      cache-references                                            
       5,95,85,981      cache-misses              #    5.450 % of all cache refs    
               153      context-switches                                            
                 0      major-faults                                                
               132      minor-faults                                                
    2,85,74,43,466      branches                                                    

     111.604092426 seconds time elapsed

       0.357567000 seconds user
       2.434672000 seconds sys

```

# Single Threaded TCP-Server and Multithreaded Client (n = 500)
```bash
 Performance counter stats for 'taskset -c 3-5 ./client 500':

      27,18,87,214      cycles                                                      
      14,36,97,916      instructions              #    0.53  insn per cycle         
       2,26,44,494      cache-references                                            
         65,90,059      cache-misses              #   29.102 % of all cache refs    
             2,456      context-switches                                            
                 0      major-faults                                                
             1,265      minor-faults                                                
       3,50,12,938      branches                                                    

     109.195354811 seconds time elapsed

       0.007509000 seconds user
       0.077425000 seconds sys
```

```bash
 Performance counter stats for 'taskset -c 0 ./server s':

    7,23,21,77,246      cycles                                                      
    7,27,98,46,688      instructions              #    1.01  insn per cycle         
      63,16,99,346      cache-references                                            
       3,29,01,795      cache-misses              #    5.208 % of all cache refs    
               120      context-switches                                            
                 0      major-faults                                                
               131      minor-faults                                                
    1,67,07,64,826      branches                                                    

     110.581629289 seconds time elapsed

       0.276137000 seconds user
       1.408317000 seconds sys

```

# Single Threaded TCP-Server and Multithreaded Client (n = 100)

```bash
 Performance counter stats for 'taskset -c 3-5 ./client 100':

       6,11,66,266      cycles                                                      
       3,04,44,784      instructions              #    0.50  insn per cycle         
         51,13,312      cache-references                                            
         17,30,856      cache-misses              #   33.850 % of all cache refs    
               317      context-switches                                            
                 0      major-faults                                                
               384      minor-faults                                                
         73,34,931      branches                                                    

       1.336215735 seconds time elapsed

       0.000000000 seconds user
       0.019437000 seconds sys
```

```bash
 Performance counter stats for 'taskset -c 0 ./server s':

    1,71,74,41,513      cycles                                                      
    1,71,80,14,476      instructions              #    1.00  insn per cycle         
      14,91,44,938      cache-references                                            
         75,89,966      cache-misses              #    5.089 % of all cache refs    
                31      context-switches                                            
                 0      major-faults                                                
               134      minor-faults                                                
      39,45,48,571      branches                                                    

       2.008441203 seconds time elapsed

       0.077491000 seconds user
       0.328315000 seconds sys
```

# Single Threaded TCP-Server and Multithreaded Client (n = 10)

```bash
 Performance counter stats for 'taskset -c 3-5 ./client 10':

         99,83,091      cycles                                                      
         55,39,075      instructions              #    0.55  insn per cycle         
          7,95,356      cache-references                                            
          3,06,633      cache-misses              #   38.553 % of all cache refs    
                34      context-switches                                            
                 0      major-faults                                                
               162      minor-faults                                                
         13,05,889      branches                                                    

       0.049723217 seconds time elapsed

       0.000000000 seconds user
       0.003228000 seconds sys
```

```bash
 Performance counter stats for 'taskset -c 0 ./server s':

      18,20,21,856      cycles                                                      
      17,68,30,561      instructions              #    0.97  insn per cycle         
       1,56,53,372      cache-references                                            
          8,58,868      cache-misses              #    5.487 % of all cache refs    
                 5      context-switches                                            
                 0      major-faults                                                
               132      minor-faults                                                
       4,06,09,487      branches                                                    

       1.247726245 seconds time elapsed

       0.012473000 seconds user
       0.037188000 seconds sys
```

---
# TCP client-server using “select” (n = 1000)

```bash
 Performance counter stats for 'taskset -c 3-5 ./client 1000':

      57,77,35,330      cycles                                                      
      29,77,86,794      instructions              #    0.52  insn per cycle         
       4,47,71,283      cache-references                                            
       1,20,82,835      cache-misses              #   26.988 % of all cache refs    
             6,276      context-switches                                            
                 0      major-faults                                                
             2,382      minor-faults                                                
       7,29,37,982      branches                                                    

     118.680815921 seconds time elapsed

       0.008450000 seconds user
       0.163251000 seconds sys
```

```bash
 Performance counter stats for 'taskset -c 0-2 ./server':

   13,09,53,90,673      cycles                                                      
   13,08,88,96,473      instructions              #    1.00  insn per cycle         
    1,11,37,36,924      cache-references                                            
       6,20,57,857      cache-misses              #    5.572 % of all cache refs    
               171      context-switches                                            
                 0      major-faults                                                
               134      minor-faults                                                
    3,00,24,95,195      branches                                                    

     119.579644057 seconds time elapsed

       0.434880000 seconds user
       2.500560000 seconds sys
```

# n = 500
```bash
 Performance counter stats for 'taskset -c 3-5 ./client 500':

      27,99,87,322      cycles                                                      
      14,55,92,409      instructions              #    0.52  insn per cycle         
       2,29,92,764      cache-references                                            
         67,93,378      cache-misses              #   29.546 % of all cache refs    
             2,382      context-switches                                            
                 0      major-faults                                                
             1,264      minor-faults                                                
       3,55,08,507      branches                                                    

      54.744675579 seconds time elapsed

       0.000000000 seconds user
       0.085662000 seconds sys
```

```bash
 Performance counter stats for 'taskset -c 0-2 ./server':

    8,79,81,73,678      cycles                                                      
    8,83,20,37,856      instructions              #    1.00  insn per cycle         
      76,32,77,886      cache-references                                            
       3,95,35,360      cache-misses              #    5.180 % of all cache refs    
                64      context-switches                                            
                 0      major-faults                                                
               129      minor-faults                                                
    2,02,66,59,494      branches                                                    

      55.778604015 seconds time elapsed

       0.266816000 seconds user
       1.657037000 seconds sys
```

# n = 100

```bash
 Performance counter stats for 'taskset -c 3-5 ./client 100':

       5,99,60,396      cycles                                                      
       3,07,12,032      instructions              #    0.51  insn per cycle         
         52,54,976      cache-references                                            
         18,16,479      cache-misses              #   34.567 % of all cache refs    
               328      context-switches                                            
                 0      major-faults                                                
               385      minor-faults                                                
         74,15,904      branches                                                    

       1.348806442 seconds time elapsed

       0.000000000 seconds user
       0.019380000 seconds sys
```

```bash
 Performance counter stats for 'taskset -c 0-2 ./server':

    1,70,43,34,179      cycles                                                      
    1,73,28,47,190      instructions              #    1.02  insn per cycle         
      14,82,52,084      cache-references                                            
         73,85,288      cache-misses              #    4.982 % of all cache refs    
                30      context-switches                                            
                 0      major-faults                                                
               133      minor-faults                                                
      39,76,59,925      branches                                                    

       2.729994392 seconds time elapsed

       0.065433000 seconds user
       0.359666000 seconds sys
```

# n = 10
```bash
 Performance counter stats for 'taskset -c 3-5 ./client 10':

       1,00,47,128      cycles                                                      
         53,73,264      instructions              #    0.53  insn per cycle         
          8,01,016      cache-references                                            
          3,02,823      cache-misses              #   37.805 % of all cache refs    
                34      context-switches                                            
                 0      major-faults                                                
               156      minor-faults                                                
         12,67,919      branches                                                    

       0.040341035 seconds time elapsed

       0.000215000 seconds user
       0.003038000 seconds sys
```

```bash
 Performance counter stats for 'taskset -c 0-2 ./server':

      18,34,60,757      cycles                                                      
      17,42,24,644      instructions              #    0.95  insn per cycle         
       1,53,68,337      cache-references                                            
          9,07,079      cache-misses              #    5.902 % of all cache refs    
                 2      context-switches                                            
                 0      major-faults                                                
               135      minor-faults                                                
       3,99,75,636      branches                                                    

       1.192340649 seconds time elapsed

       0.004467000 seconds user
       0.035739000 seconds sys
```
---

