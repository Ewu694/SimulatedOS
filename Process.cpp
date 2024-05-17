#include "Process.h"

Process::Process() = default;

int Process::getPID() const{
  return PID;
}

int Process::getState(){
  return pState;
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
