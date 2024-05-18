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
    bool isChild(Process process); //checks if a process is a child or parent
    void findParentAndTerminate();//if the current process is a parent, terminate all of its children and then run top of readyq
    void findChildAndTerminate();//if the current process is a child, terminate it from the processes vector and then run top of readyq
    void cascadingTermination(Process &process);//gets parent of current process, terminates all of its current children and adds parent to ready queue
    void interrupt();
    void setCurrentProcess(Process PID);

  private:
    std::deque<int> ready_queue;
    int CPU;
    Process currProcess;
    int processCount = 1; //used to set PIDs and PIDs start from 1
    std::vector<Process> Processes;
    std::vector<Process> Children;
};

#endif
