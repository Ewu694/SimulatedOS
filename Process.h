#ifndef PROCESS_H
#define PROCESS_H

constexpr int NO_PROCESS{0};
constexpr int READY{1};
constexpr int RUNNING{2};
constexpr int WAITING{3};
constexpr int TERMINATED{4};

enum TYPE{CHILD, PARENT, ZOMBIE};

#include <iostream>

class Process{
  public:
    Process();
    int getPID() const;    
    int getState();
    TYPE getType();
    void setPID(int newPID);
    void setState(int currentState);
    void setType(TYPE currentType);
  private:
    int PID;
    int pState;
    TYPE pType;
};

#endif