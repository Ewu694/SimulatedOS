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

void CPUManager::addToReadyQueue(Process &newProcess){//if the CPU is empty, set it to the running process, else add it to the ready queue
  if(ready_queue.empty()){
    newProcess.setState(2);
    setCurrentProcess(newProcess);
  }
  else{
    ready_queue.push_back(newProcess.getPID());
    newProcess.setState(1);
  }
}

int CPUManager::getCPUProcess(){
  return CPU;
}

Process CPUManager::getCurrentProcess(){ //returns PID of current process
  return currProcess;
}

void CPUManager::setCurrentProcess(Process process){
  currProcess = process;
  CPU = process.getPID();
}

void CPUManager::exitProcess(){//sets the process in the front of the ready queue to running and pops it from the top of ready queue
  if(currProcess.getType() == PARENT){
    findParentAndTerminate();
  }
  if(currProcess.getType() == CHILD){
     findChildAndTerminate();
  }
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
  if(CPU == 0){
    newProcess.setState(2);
  }
  else if(CPU != 0){
    newProcess.setState(1);
  }
  addToReadyQueue(newProcess);
  Processes.push_back(newProcess);
  processCount++;
}

void CPUManager::forkProcess(){
  if(CPU != 0){
    Process child;   
    child.setPID(processCount);
    for(int i = 0; i < Processes.size(); ++i){
      if(Processes[i].getPID() == CPU){
        Processes[i].setType(PARENT);
        child.setType(CHILD);
        child.setParent(CPU);
      }
    }
    Processes.push_back(child);
    ready_queue.push_back(child.getPID());
    processCount++;
  }
}

bool CPUManager::isChild(Process process){
  for(int i = 0; i < Children.size(); ++i){
    if(Children[i].getPID() == process.getPID()){
      return true;
    }
  }
  return false;
}

void CPUManager::cascadingTermination(Process &process){//loops through children, if the param process is found to be a child, recursively call function to see if it has any children, when it reaches a process with no children, it will return it
  int processLocation = 0;
  bool isChildProcess = false;
  for(int child = 0; child < Children.size(); ++child){
    if(process.getPID() == Children[child].getPID()){ 
      isChildProcess = true;
    }
  }
  if(isChildProcess){
    for(int it = 0; it < Processes.size(); ++it){
      if(Processes[it].getPID() == process.getParentPID() && Processes[it].getState() != 3){//if the parent process of this process is not waiting for it to terminaet then:
        process.setType(ZOMBIE);
        return;
      }
      else if(Processes[it].getPID() == process.getParentPID() && Processes[it].getState() == 3){
        for(int i = 0; i < Children.size(); ++i){
          for(int parentIterator = 0; parentIterator < Processes.size(); ++parentIterator){
            if(Processes[parentIterator].getPID() == process.getPID())//indexes where the param process is to delete later
              processLocation = parentIterator;
            if(Processes[parentIterator].getParentPID() == process.getPID() && (Processes[parentIterator].getType() == PARENT))//if a child of one of the processes is also a parent then, recursively find a child and work up from there to delete it, after that it is done, we will delete the parent too
              cascadingTermination(Processes[parentIterator]);
            else if(Processes[parentIterator].getType() == CHILD){
              for(int j = 0; j < Children.size(); ++j){
                if(Processes[parentIterator].getPID() == Children[j].getPID())
                  Children.erase(Children.begin() + j);
              }
              Processes[parentIterator].setState(4);
              Processes.erase(Processes.begin() + parentIterator);
            }
          }
          Processes[processLocation].setState(4);
          Processes.erase(Processes.begin() + processLocation);//delete parent from process vector
          Children.erase(Children.begin() + i);//delete process from children vector
        }
      }
    }
  }
  else{
    for(int parentIterator = 0; parentIterator < Processes.size(); ++parentIterator){
      if(Processes[parentIterator].getPID() == process.getPID())//indexes where the param process is to delete later
        processLocation = parentIterator;
      if(Processes[parentIterator].getParentPID() == process.getPID() && (Processes[parentIterator].getType() == PARENT))//if a child of one of the processes is also a parent then, recursively find a child and work up from there to delete it, after that it is done, we will delete the parent too
        cascadingTermination(Processes[parentIterator]);
      else if(Processes[parentIterator].getType() == CHILD){
        for(int j = 0; j < Children.size(); ++j){
          if(Processes[parentIterator].getPID() == Children[j].getPID())
            Children.erase(Children.begin() + j);
        }
        Processes[parentIterator].setState(4);
        Processes.erase(Processes.begin() + parentIterator);
      }
    }
    Processes[processLocation].setState(4);
    Processes.erase(Processes.begin() + processLocation);//delete parent from process vector
  }
}

void CPUManager::findParentAndTerminate(){
  cascadingTermination(currProcess);
  for(int i = 0; i < Processes.size(); ++i){
    if(Processes[i].getPID() == ready_queue.front()){//if this process is the one on top of the ready queue, set it to current process and pop it off the ready queue
      setCurrentProcess(Processes[i]);
      ready_queue.pop_front(); 
      return;
    }
  }
}

void CPUManager::findChildAndTerminate(){
  for(int i = 0; i < Children.size(); ++i){
    if(Children[i].getPID() == currProcess.getPID()){
      Children.erase(Children.begin() + i);
    }
  }
  for(int it = 0; it < Processes.size(); ++it){
      if(Processes[it].getPID() == currProcess.getParentPID() && Processes[it].getState() != 3){//if the parent process of this process is not waiting for it to terminaet then:
        currProcess.setType(ZOMBIE);
      }
      else if(Processes[it].getPID() == CPU && Processes[it].getState() == 3){
        Processes[it].setState(4);
        Processes.erase(Processes.begin() + it);
      }
      if(Processes[it].getPID() == ready_queue.front()){
        setCurrentProcess(Processes[it]);
        ready_queue.pop_front();
      }
  }
}

void CPUManager::interrupt(){
  if(!ready_queue.empty()){//if ready queue is not empty
  //first loop through all processes to pause the current process if the stack is not empty
  for(int i = 0; i < Processes.size(); ++i){
      if(Processes[i].getPID() == CPU){//if the current process has matching PID to the current Process
        Processes[i].setState(1);//pause it by settings its state to ready
        ready_queue.push_back(Processes[i].getPID());//add it to the back of the ready queue
      }
  }
  //loop through processes again to find the process at the top of the ready queue and set it to running and adding it to 
  for(int i = 0; i < Processes.size(); ++i){
      if(Processes[i].getPID() == ready_queue.front()){//if current process is the one in front of the ready queue
        Processes[i].setState(2);//set the process at the type of ready queue to running
        setCurrentProcess(Processes[i]);//set the currently running process to the process at the top of the ready queue
        ready_queue.pop_front();//pop it off the ready queue now that its running
      }
    }
  }
}



