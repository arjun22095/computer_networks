# CN PA-02 2024
**Arjun Tandon (2022095)** | **Arav Amawate (22091)**
---
*Disclaimer : Here the execution time of the client is used as the execution time of the server because the server is based on SIGINT action and includes appropriate handler for the same as well.
Whereas client starts after the server is started and listening, and sends a specified number of requests to the server, receives a response from the server (the proc data) in this case,
and exits instantly upon receving it. So the execution time of the client has to be taken in consideration while comparing corressponding TCP clients (multi-threaded, single-threaded, using-select),
as their execution time has human error of manually using SIGINT to end the program.*
---
# Report

We discuss the stats we measured

# Cycles : 
**General Trend** : Concurrent > Select > Single-Threaded
Even though select also uses a single thread, it is able to outperform the vanilla Single-Threaded server because the `select` function doesn't block and also doesn't have the overhead that comes due to `multithreading`. It sits in the middle of concurrent server and single-threaded server despite using the same resources as a single-threaded server, all thanks to it's non-blocking behaviour.

# IPC (Instructions/Cycle): 
**General Trend** : Single-Threaded ≈ Select > Concurrent
Concurrent client gets a higher IPC when the number of clients are large, that is when it is able to take advantage of the multithreading capabilities provided by `pthread` and the multi-threading benefits overpower the overheads that are incurred while multi-threading.

# Cache References
**General Trend** : Concurrent > Single-threaded > Select
In concurrent programs, multiple threads operate simultaneously and often the same data is being processed in parallel, which increases the frequency of data fetch from CPU cache. Select also processes one socket at time but continuously does polling, meaning that it continuously checks a set of sockets to determine if they are ready for reading, writing, or have encountered errors which causes a lot of cache requests.

# Cache Misses
**General Trend** : Concurrent > Single-Threaded ≈ Select
As the number of cache references are high for multi-threaded server as we discussed above, it also causes more frequent eviction of the cache blocks.

# Context Switches
**General Trend** : Concurrent > Single-Threaded > Select
Concurrent server creates a new thread for every client request and for a large amount of client requests, the number of threads created are high as well, so since the number of threads are high, the CPU scheduler will make all of them run and give us the feeling as if they are running concurrently (similar to what we study in OS, round-robin or other scheduling policies) which are based on the principle and queuing the tasks (generally processes but threads in this case), letting them loading them (context switch), run for a time-slice (or CPU burst) and evicting them and then context switching to another thread. So the number of context switches are extremely high in case of concurrent servers. They are much lower for single-threaded servers but even lower for select based servers because select based servers are non-blocking whereas single-threaded servers use the blocking call which causes the CPU to context switch to another task. 

# Execution Time
**General Trend** : Single-Threaded > Select > Concurrent
Parallelisation which is done by the concurrent TCP server does come at the cost of a lot of overheads (context switching and cache misses) but is able to utilise much more CPU resources than it's single-threaded counterparts like select-based server and the single-threaded server. The select-based server lies almost in the middle of single-threaded and concurrent servers because of non-blocking and being able to handle clients sequentially

So for pure performance, multi-threading is the way to go for higher number of clients. However select offers a good compromise by using lesser resources (only a single thread, lesser overheads) than the concurrent server and still yielding a much better performance than the single-threaded server.

---
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
# TCP client-server using “select” (n = 500)
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

# TCP client-server using “select” (n = 100)

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

# TCP client-server using “select” (n = 10)
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

# TCP client-server using “select” (n = 2)

```bash
 Performance counter stats for 'taskset -c 3-5 ./client 2':

         43,37,846      cycles                                                      
         27,39,809      instructions              #    0.63  insn per cycle         
          3,02,002      cache-references                                            
          1,15,377      cache-misses              #   38.204 % of all cache refs    
                 7      context-switches                                            
                 0      major-faults                                                
               135      minor-faults                                                
          6,33,672      branches                                                    

       0.013384890 seconds time elapsed

       0.001611000 seconds user
       0.000000000 seconds sys
```

```bash
 Performance counter stats for 'taskset -c 0-2 ./server':

       4,09,07,776      cycles                                                      
       3,70,26,166      instructions              #    0.91  insn per cycle         
         32,44,820      cache-references                                            
          2,22,997      cache-misses              #    6.872 % of all cache refs    
                 2      context-switches                                            
                 0      major-faults                                                
               131      minor-faults                                                
         84,96,649      branches                                                    

       1.057495326 seconds time elapsed

       0.000000000 seconds user
       0.013611000 seconds sys
```
