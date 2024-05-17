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
    int getCurrentProcess(); //returns the process currently using the CPU
    void exitProcess(); //terminates from current process and pushes the next process in ready queue to CPU
    void addToReadyQueue(Process process);//if ready queue is empty send PID to CPU, else add to ready queue
    bool isReadyQueueEmpty(); //checks if there is nothing in ready queue
    void createProcess(); //creates new process
    void forkProcess(); //forks from parent process 
    bool isChild(Process process); //checks if a process is a child or parent
    void setCurrentProcess(Process process);
    void findParentAndTerminate();//gets its parent and terminate itself
    void findChildrenAndTerminate();//gets parent of current process, terminates all of its current children and adds parent to ready queue
    void interrupt();

  private:
    std::deque<int> ready_queue;
    int CPU;
    int processCount = 1; //used to set PIDs and PIDs start from 1
    Process currProcess;
    std::vector<Process> Processes;
    std::vector<Process> Children;
};

#endif
