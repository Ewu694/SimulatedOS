#include <CPUManager.hpp>

CPUManager::CPUManager(){
  CPU = 0;
}

CPUManager::CPUManager(int CPU){
  ready_queue.push_front(CPU);
}

std::deque<int> CPUManager::getReadyQueue(){
  return ready_queue;
}

void CPUManager::AddToReadyQueue(int PID){
  if(ready_queue.empty()){
    CPU = PID;
  }
  else{
    ready_queue.push_back(PID);
  }
}

int CPUManager::getCurrentProcess(){
  return CPU;
}

void CPUManager::exitProcess(){
  CPU = ready_queue.front();
  ready_queue.pop_back();
}