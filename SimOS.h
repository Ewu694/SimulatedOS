//Eric Wu
#ifndef SIMOS_H
#define SIMOS_H

#include <iostream>
#include <deque>
#include <vector>
#include <unordered_map>
#include "CPUManager.h"
#include "DiskManager.h"
#include "MemoryManager.h"
#include "Process.h"
 
class SimOS{
  public: 
    SimOS(int numberOfDisks, unsigned long long ramSize, unsigned int pageSize);
    void NewProcess(); // Creates a new process in the simulated system. The new process takes place in the ready-queue or immediately starts using the CPU. Every process in the simulated system has a PID. Your simulation assigns PIDs to new processes starting from 1 and increments it by one for each new process. Do not reuse PIDs of the terminated processes.
    void SimFork(); //The currently running process forks a child. The child is placed in the end of the ready-queue.
    void SimExit(); //The process that is currently using the CPU terminates. Make sure you release the memory used by this process immediately. If its parent is already waiting, the process terminates immediately and the parent becomes runnable (goes to the ready-queue). If its parent hasn't called wait yet, the process turns into zombie. To avoid the appearance of the orphans, the system implements the cascading termination. Cascading termination means that if a process terminates, all its descendants terminate with it.
    void SimWait(); //The process wants to pause and wait for any of its child processes to terminate. Once the wait is over, the process goes to the end of the ready-queue or the CPU. If the zombie-child already exists, the process proceeds right away (keeps using the CPU) and the zombie-child disappears. If more than one zombie-child exists, the system uses one of them (any!) to immediately resumes the parent, while other zombies keep waiting for the next wait from the parent
    void TimerInterrupt(); //Interrupt arrives from the timer signaling that the time slice of the currently running process is over.
    void DiskReadRequest(int diskNumber, std::string fileName); //Currently running process requests to read the specified file from the disk with a given number. The process issuing disk reading requests immediately stops using the CPU, even if the ready-queue is empty.
    void DiskJobCompleted(int diskNumber); //A disk with a specified number reports that a single job is completed. The served process should return to the ready-queue.
    void AccessMemoryAddress(unsigned long long address); //Currently running process wants to access the specified logical memory address. System makes sure the corresponding page is loaded in the RAM. If the corresponding page is already in the RAM, its “recently used” information is updated.
    int GetCPU(); //GetCPU returns the PID of the process currently using the CPU. If CPU is idle it returns NO_PROCESS 
    std::deque<int> GetReadyQueue(); //GetReadyQueue returns the std::deque with PIDs of processes in the ready-queue where element in front corresponds start of the ready-queue.
    MemoryUsage GetMemory(); //GetMemory returns MemoryUsage vector describing all currently used frames of RAM. Remember, Terminated “zombie” processes don’t use memory, so they don’t contribute to memory usage. MemoryItems appear in the MemoryUsage vector in the order they appear in memory (from low addresses to high).
    FileReadRequest GetDisk(int diskNumber); //GetDisk returns an object with PID of the process served by specified disk and the name of the file read for that process. If the disk is idle, GetDisk returns the default FileReadRequest object (with PID 0 and empty string in fileName)
    std::deque<FileReadRequest> GetDiskQueue(int diskNumber); //GetDiskQueue returns the I/O-queue of the specified disk starting from the “next to be served” process.
  private:  
    int numberOfDisks_ = 0;
    unsigned long long amountOfRam_ = 0; 
    unsigned int pageSize_ = 0;
    CPUManager CPU_;
    DiskManager disks_;
    MemoryManager RAM_;
};
#endif