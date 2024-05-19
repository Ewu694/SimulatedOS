#ifndef CPUMANAGER_H
#define CPUMANAGER_H

#include <deque>
#include <vector>
#include "Process.h"

class CPUManager{
  public:
    CPUManager();
    CPUManager(int CPU); //if CPU already has a running process add it 
    std::deque<int> getReadyQueue(); //returns the ready queue 
    int getCPUProcess();
    Process getCurrentProcess(); //returns the process currently using the CPU
    void exitProcess(); //terminates from current process and pushes the next process in ready queue to CPU
    void addToReadyQueue(Process &process);//if ready queue is empty send PID to CPU, else add to ready queue
    bool isReadyQueueEmpty(); //checks if there is nothing in ready queue
    void createProcess(); //creates new process
    void forkProcess(); //forks from parent process 
    bool isParent(Process process); //checks if a process is a child or parent
    bool hasZombie(std::vector<Process> allProcesses); //checks if zombie processes exist within a vector of processes
    void findParentAndTerminate();//if the current process is a parent, terminate all of its children and then run top of readyq
    void findChildAndTerminate();//if the current process is a child, terminate it from the processes vector and then run top of readyq
    void cascadingTerminationExit(Process &process);//gets parent of current process, terminates all of its current children, itself, and adds parent to ready queue
    void cascadingTerminationWait(Process &process);
    void interrupt();
    void setCurrentProcess(Process PID);
    void runFirstProcess();
    void wait();
  private:
    std::deque<int> ready_queue;
    int CPU;
    Process currProcess;
    int processCount = 1; //used to set PIDs and PIDs start from 1
    std::vector<Process> Processes;
    std::vector<Process> Children;
    std::vector<Process> Zombies;
};

#endif
