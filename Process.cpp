//Eric Wu
#include "Process.h"

Process::Process(){
  PID = 0;
  pState = NO_PROCESS;
  parentPID = 0;
}

Process::Process(const int& processID, const int& processState){
  PID = processID;
  pState = processState;
  parentPID = 0;
}

Process& Process::operator=(const Process& process)
{
    PID = process.PID;
    pState = process.pState;
    pType = process.pType;
    parentPID = process.parentPID;
    children_ = process.children_;
    return *this;
}
int Process::getPID(){
  return PID;
}

int Process::getState(){
  return pState;
}

TYPE Process::getType(){
  return pType;
}

int Process::getParentPID(){
  return parentPID;
}

std::vector<int> Process::getChildren(){
  return children_;
}

void Process::setPID(int newPID){
  PID = newPID;
}

void Process::setState(int currentState){
  pState = currentState;
}

void Process::setType(TYPE processType){
  pType = processType;
}

void Process::setParent(int PID){
  parentPID = PID;
}

void Process::addChild(int PID){
  children_.push_back(PID);
}

bool Process::hasChildren(Process process){
  if(!process.getChildren().empty())
    return true;
  else
    return false;
}

void Process::removeChild(int index){
  children_.erase(children_.begin() + index);
}