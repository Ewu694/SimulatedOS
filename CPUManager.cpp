#include "CPUManager.h"

CPUManager::CPUManager(){
  CPU = 0;
  processCount = 1;
}

CPUManager::CPUManager(int CPU){
  ready_queue.push_front(CPU);
}

std::deque<int> CPUManager::getReadyQueue(){
  return ready_queue;
}

void CPUManager::setCurrentProcess(Process process){
  currProcess = process;
}

void CPUManager::addToReadyQueue(Process &newProcess){//if the CPU is empty, set it to the running process, else add it to the ready queue
  if(ready_queue.empty()){
    CPU = newProcess.getPID();
    newProcess.setState(2);
    setCurrentProcess(newProcess);
  }
  else{
    ready_queue.push_back(newProcess.getPID());
    newProcess.setState(1);
  }
}

int CPUManager::getCurrentProcess(){ //returns PID of current process
  return CPU;
}

void CPUManager::exitProcess(){//sets the process in the front of the ready queue to running and pops it from the top of ready queue
  if(currProcess.getType() == PARENT){
    findParent();
  }
  if(currProcess.getType() == CHILD){
     findChild();
  }
  CPU = ready_queue.front();
  ready_queue.pop_front(); 
}


bool CPUManager::isReadyQueueEmpty(){
  if(ready_queue.empty())
    return true;
  else
    return false;
}

void CPUManager::createProcess(){
  Process newProcess;
  newProcess.setPID(processCount);
  if(getCurrentProcess() == 0){
    newProcess.setState(2);
  }
  else if(getCurrentProcess() != 0){
    newProcess.setState(1);
  }
  addToReadyQueue(newProcess);
  Processes.push_back(newProcess);
  processCount++;
}

void CPUManager::forkProcess(){
  Process child;   
  child.setPID(processCount);
  if(getCurrentProcess() == 0){
    child.setState(2);
  }
  else if(getCurrentProcess() != 0){ 
    child.setState(1);
  }
  addToReadyQueue(child);
  for(int i = 0; i < Processes.size(); ++i){
    if(Processes[i].getPID() == getCurrentProcess()){
      Processes[i].setType(PARENT);
      child.setType(CHILD);
      Children.insert({child, Processes[i].getPID()});
    }
  }
  processCount++;
}

bool CPUManager::isChild(Process process){
  for(auto it = Children.begin(); it != Children.end(); ++it){
    if(it->first.getPID() == process.getPID()){
      return true;
      break;
    }
  }
}

void CPUManager::findParent(){
  int i = 0; //init parser for processes vector
  for(auto it = Children.begin(); it != Children.end(); ++it){
    if((currProcess.getPID() == it->first.getPID()) && it->second == Processes[i].getPID() && (Processes[i].getState() == WAITING)){ //if this is the parent of the current child process then 
      int j = 0;
      for(auto childIter = Children.begin(); childIter != Children.end(); ++childIter){//terminates children of current process
        if((currProcess.getPID() == childIter->second) && childIter->first.getPID() == Processes[j].getPID()){//if this current process is the parent, terminate its child
          Processes.erase(Processes.begin() + j);
        }
        ++j;
      }
      Processes.erase(Processes.begin() + i);//terminate current process
      addToReadyQueue(Processes[i]);
      break;
      }
    ++i;
  }
}

void CPUManager::findChild(){
  int i = 0; //init parser for processes vector
  for(auto it = Children.begin(); it != Children.end(); ++it){
      if((currProcess.getPID() == it->first.getPID()) && it->second == Processes[i].getPID()){//if matching child and parent, termiante it and add the parent to ready queue
        Processes.erase(Processes.begin() + i);
        addToReadyQueue(Processes[i]);
        break;
      }
  }
}

void CPUManager::interrupt(){
  //first loop through all processes to pause the current process if the stack is not empty
  for(int i = 0; i < Processes.size(); ++i){
    if(!ready_queue.empty()){//if ready queue is not empty
      if(Processes[i].getPID() == getCurrentProcess()){//if the current process has the matching PID to the current Process
        Processes[i].setState(1);//pause it by settings its state to running
        addToReadyQueue(Processes[i]);//add it to the back of the ready queue
      }
    else//else if its empty, terminate function and let process continue running
      return;
    }
  }
  //loop through processes again to find the process at the top of the ready queue and set it to running and adding it to 
  for(int i = 0; i < Processes.size(); ++i){
      if(Processes[i].getPID() == ready_queue.front()){//if current process is the one in front of the ready queue
        Processes[i].setState(2);//set the process at the type of ready queue to running
        currProcess = Processes[i];//set the currently running process to the process at the top of the ready queue
        CPU = currProcess.getPID();//run the top of ready queue by setting CPU to it
        ready_queue.pop_front();//pop it off the ready queue now that its running
      }
  }
}


