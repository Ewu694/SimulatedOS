#ifndef CPUMANAGER_HPP
#define CPUMANAGER_HPP

#include <iostream>
#include <deque>

class CPUManager{
  public:
    CPUManager();
    CPUManager(int CPU);
    void AddToReadyQueue(int PID);//if ready queue is empty send PID to CPU, else add to ready queue
    std::deque<int> getReadyQueue();
    int getCurrentProcess();

  private:
    std::deque<int> ready_queue;
    std::unordered_map<int, int> PID;
    int CPU;
};

#endif
