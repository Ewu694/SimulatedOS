#ifndef PROCESS_HPP
#define PROCESS_HPP

enum STATE{NEW, READY, RUNNING, WAITING, TERMINATED}; //diff type of states for processes

#include <iostream>
#include <unordered_map> 

class Process{
  public:
    int getPID();
    void setPID(int newPID);
    STATE getState();
    void setState(STATE currentState);

  private:
    int PID;
    STATE pState;
    
};

#endif