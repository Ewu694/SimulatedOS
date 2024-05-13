#include <Process.hpp>

int Process::getPID(){
  return PID;
}

void Process::setPID(int newPID){
  PID = newPID;
}

STATE Process::getState(){
  return pState;
}

void Process::setState(STATE currentState){
  pState = currentState;
}

