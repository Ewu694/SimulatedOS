//Eric Wu
#ifndef PROCESS_H
#define PROCESS_H

constexpr int NO_PROCESS{0};
constexpr int READY{1};
constexpr int RUNNING{2};
constexpr int WAITING{3};
constexpr int TERMINATED{4};

enum TYPE{REGULAR, CHILD, PARENT, ZOMBIE};

#include <iostream>
#include <vector>

class Process{
  public:
    Process();
    Process(const int& processID, const int& processState);
    Process& operator=(const Process& process);   
    int getPID();    
    int getState();
    TYPE getType();
    int getParentPID();
    std::vector<int> getChildren();
    void setPID(int newPID);
    void setState(int currentState);
    void setType(TYPE currentType);
    void setParent(int PID);
    void addChild(int PID);
    bool hasChildren(Process process);
    void removeChild(int index);
  private:
    int PID;
    int pState;
    TYPE pType;
    int parentPID;
    std::vector<int> children_;

};

#endif