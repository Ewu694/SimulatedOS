//Eric Wu
#ifndef CPUMANAGER_H
#define CPUMANAGER_H

#include <deque>
#include "Process.h"

class CPUManager{
  public:
    CPUManager();
    CPUManager(int CPU); //if CPU already has a running process add it 
    std::deque<int> getReadyQueue(); //returns the ready queue 
    int getCPUProcess();
    Process getCurrentProcess(); //returns the process currently using the CPU
    void exitProcess(); //terminates from current process and pushes the next process in ready queue to CPU
    void addToReadyQueue(int PID);//if ready queue is empty send PID to CPU, else add to ready queue
    bool isReadyQueueEmpty(); //checks if there is nothing in ready queue
    void createProcess(); //creates new process
    void forkProcess(); //forks from parent process 
    bool hasZombie(std::vector<Process> allProcesses); //checks if zombie processes exist within a vector of processes
    void cascadingTerminationExit();//gets parent of current process, terminates all of its current children, itself, and adds parent to ready queue
    void cascadingTerminationWait();
    void interrupt();
    void setCurrentProcess(Process PID);
    void runFirstProcess();
    void wait();
    void setCPUState(int STATE);
  private:
    std::deque<int> readyQueue_;
    int CPU_;
    Process currProcess_;
    int processCount_ = 1; //used to set PIDs and PIDs start from 1
    std::vector<Process> processes_;
    std::vector<Process> zombies_;
};

#endif
