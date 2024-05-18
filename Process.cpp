#include "Process.h"

Process::Process() = default;

Process& Process::operator=(const Process& process)
{
    PID = process.PID;
    pState = process.pState;
    pType = process.pType;
    parentPID= process.parentPID;
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
