#Simulated Operating System
A simulation of a operating system made using C++. This simulation of the operating system includes CPU, Memory, and Disk functions performed by the operating system <br>
This was done without the use of:
- The goto operator
- Non-constant global variables
- C-style arrays
- Explicit dynamic memory allocation


##Notes for CPU
- CPU scheduling is round-robin. This Ready-queue is a real first-come-first-serve queue. Each process has a time limit for how long it can use the CPU at once. If a process uses the CPU for a longer time, it goes back to the end of the ready-queue.
<br>

##Notes for Memory
- For memory management, our simulation uses paging. If the memory is full, the least recently used frame is removed from memory.
<br>

##Notes Disk
- Disk management is “first-come-first-served”. In other words, all disk I/O-queues are real queues (FIFO)
<br>

#Further detail on what each function does has been included within the files themselves

##Tech Stack
- C++ 
