#ifndef CPUMANAGER_HPP
#define CPUMANAGER_HPP

#include <iostream>
#include <deque>

class CPUManager{
  public:
    CPUManager();
    CPUManager(int CPU);
    void addToReadyQueue(int PID);//if ready queue is empty send PID to CPU, else add to ready queue
    std::deque<int> getReadyQueue();
    int getCurrentProcess();
    void exitProcess();
    bool isReadyQueueEmpty();

  private:
    std::deque<int> ready_queue;
    int CPU;
};

#endif
