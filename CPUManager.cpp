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

void CPUManager::addToReadyQueue(Process newProcess){//if the CPU is empty, set it to the running process, else add it to the ready queue
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
    findChildrenAndTerminate();
  }
  if(currProcess.getType() == CHILD){
     findParentAndTerminate();
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
  newProcess.setType(REGULAR);
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
  child.setState(1);
  addToReadyQueue(child);
  for(int i = 0; i < Processes.size(); ++i){
    if(Processes[i].getPID() == getCurrentProcess()){
      Processes[i].setType(PARENT);
      child.setType(CHILD);
      child.setParent(getCurrentProcess());
    }
  }
  processCount++;
}

bool CPUManager::isChild(Process process){
  for(int i = 0; i < Children.size(); ++i){
    if(Children[i].getPID() == process.getPID()){
      return true;
    }
  }
  return false;
}

void CPUManager::findChildrenAndTerminate(){
  int parentIterator = 0;
  for(int i = 0; i < Children.size(); ++i){
    if(currProcess.getPID() == Children[i].getPID() && Children[i].getParentPID() == Processes[parentIterator].getPID() && (Processes[parentIterator].getState() == WAITING)){ //if this is the parent of the current child process then 
      int childIterator = 0;
      for(int j = 0; j < Children.size(); ++j){//terminates children of current process
        if((currProcess.getPID() == Children[j].getParentPID()) && Children[j].getPID() == Processes[childIterator].getPID()){//if this current process is the parent, terminate its child
          Processes.erase(Processes.begin() + childIterator);
        }
        ++childIterator;
      }
      Processes.erase(Processes.begin() + parentIterator);//terminate current process
      addToReadyQueue(Processes[parentIterator]);
      break;
      }
    ++parentIterator;
  }
}

void CPUManager::findParentAndTerminate(){
  int parentIterator = 0; //init parser for processes vector
  for(int i = 0; i < Children.size(); ++i){
      if((currProcess.getPID() == Children[i].getPID()) && Children[i].getParentPID() == Processes[parentIterator].getPID()){//if matching child and parent, termiante it and add the parent to ready queue
        Processes.erase(Processes.begin() + parentIterator);
        addToReadyQueue(Processes[parentIterator]);
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


