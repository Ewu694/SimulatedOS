#include "Process.h"

Process::Process() = default;

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
