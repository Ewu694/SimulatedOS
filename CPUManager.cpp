//Eric Wu
#include "CPUManager.h"

CPUManager::CPUManager(){
  CPU_ = 0;
  processCount_ = 1;
}

CPUManager::CPUManager(int CPU){
  readyQueue_.push_front(CPU);
}

std::deque<int> CPUManager::getReadyQueue(){
  return readyQueue_;
}

void CPUManager::addToReadyQueue(int PID){//if the CPU is empty, set it to the running process, else add it to the ready queue
  if(readyQueue_.empty()){
    for(int i = 0; i < processes_.size(); ++i){
      if(processes_[i].getPID() == PID){
        processes_[i].setState(RUNNING);
        setCurrentProcess(processes_[i]);
      }
    }
  }
  else{
    for(int i = 0; i < processes_.size(); ++i){
      if(processes_[i].getPID() == PID){
        processes_[i].setState(READY);
        readyQueue_.push_back(PID);
      }
    }
  }
}

void CPUManager::runFirstProcess(){
  if(!readyQueue_.empty()){
    for(int i = 0; i < processes_.size(); ++i){
      if(processes_[i].getPID() == readyQueue_.front()){
        processes_[i].setState(RUNNING);
        setCurrentProcess(processes_[i]);
        break;
      }
    }
    readyQueue_.pop_front();
  }
}

int CPUManager::getCPUProcess(){
  if(CPU_ == 0){
    return NO_PROCESS;
  }
  return CPU_;
}

Process CPUManager::getCurrentProcess(){ //returns PID of current process
  return currProcess_;
}

void CPUManager::setCurrentProcess(Process process){
  currProcess_ = process;
  CPU_ = process.getPID();
}

void CPUManager::exitProcess(){//sets the process in the front of the ready queue to running and pops it from the top of ready queue
  cascadingTerminationExit();
}


bool CPUManager::isReadyQueueEmpty(){
  if(readyQueue_.empty())
    return true;
  else
    return false;
}

void CPUManager::createProcess(){
  Process newProcess;
  newProcess.setType(REGULAR);
  newProcess.setPID(processCount_);
  processes_.push_back(newProcess);
  addToReadyQueue(newProcess.getPID());
  processCount_++;
}

void CPUManager::forkProcess(){
  if(CPU_ != 0){
    Process child;   
    child.setPID(processCount_);
    for(int i = 0; i < processes_.size(); ++i){
      if(processes_[i].getPID() == CPU_){
        processes_[i].setType(PARENT);
        processes_[i].addChild(child.getPID());
        child.setType(CHILD);
        child.setParent(CPU_);
      }
    }
    processes_.push_back(child);
    readyQueue_.push_back(child.getPID());
    processCount_++;
  }
  else
    throw std::logic_error("This instruction requires a running process");
}

void CPUManager::cascadingTerminationExit(){//loops through children, if the param process is found to be a child, recursively call function to see if it has any children, when it reaches a process with no children, it will return it
  if(currProcess_.getParentPID() != 0){
    int parentIndex = 0;
    for(int it = 0; it < processes_.size(); ++it){
      if(processes_[it].getPID() == currProcess_.getParentPID() && processes_[it].getState() != 3){//if the parent process of this process is not waiting for it to terminate then terminate its children and make it a zombie (now in the future zombie processes can just be terminated without worrying about if it has children)
        int childIterator = 0;
        while(!currProcess_.getChildren().empty() && childIterator < currProcess_.getChildren().size()){//while this process has children 
          for(int i = 0; i < processes_.size(); ++i){//loop through all the processes
            if(currProcess_.getChildren()[childIterator] == processes_[i].getPID()){//if this process is the child of the param process 
              if(!processes_[i].getChildren().empty()){//check if it also has children
                int childsChildIterator = 0;
                while(childsChildIterator < processes_[i].getChildren().size()){//if it does, loop through processes to find its children and add it into the param   processes's children to be deleted 
                  for(int j = 0; j < processes_.size(); ++j){
                    currProcess_.addChild(processes_[j].getPID());
                  }
                }
                processes_[i].getChildren().erase(processes_[i].getChildren().begin() + i);//delete the child of the param process after all its children are added
              }
              currProcess_.removeChild(childIterator);//remove child of param process 
            }
          }
        }
        currProcess_.setType(ZOMBIE);
        currProcess_.setState(TERMINATED);
        zombies_.push_back(currProcess_);

        runFirstProcess();
        break;//break after deleting the zombie child's children
      }
      else if(processes_[it].getPID() == currProcess_.getParentPID() && processes_[it].getState() == 3){//else if the parent is waiting 
        parentIndex = it;
        int childIterator = 0;
        int processIndex = 0;
        while(!currProcess_.getChildren().empty() && childIterator < currProcess_.getChildren().size()){//while this process has children 
          for(int i = 0; i < processes_.size(); ++i){//loop through all the processes
            if(currProcess_.getPID() == processes_[i].getPID())
              processIndex = i;
            if(currProcess_.getChildren()[childIterator] == processes_[i].getPID()){//if this process is the child of the param process 
              if(!processes_[i].getChildren().empty()){//check if it also has children
                int childsChildIterator = 0;
                while(childsChildIterator < processes_[i].getChildren().size()){//if it does, loop through processes to find its children and add it into the param processes's children to be deleted 
                  for(int j = 0; j < processes_.size(); ++j){
                    currProcess_.addChild(processes_[j].getPID());
                  }
                }
                processes_[i].getChildren().erase(processes_[i].getChildren().begin() + i);//delete the child of the param process after all its children are added
              }
              currProcess_.removeChild(childIterator);//remove it from the vector of children
            }
          }
        }
        processes_.erase(processes_.begin() + processIndex);//delete the current process to exit from it
        runFirstProcess();
        addToReadyQueue(processes_[it].getPID());//pushes parent to the back of ready queue or CPU
        break;//break after deleting all the children of the curr process and adding parent to ready queue
      }
    }
  }
  else{//if just a parent process and not a child to any process then:
    int childIterator = 0;
    int processIndex = 0;
    while(!currProcess_.getChildren().empty() && childIterator < currProcess_.getChildren().size()){//while this process has children 
      for(int i = 0; i < processes_.size(); ++i){//loop through all the processes
        if(currProcess_.getPID() == processes_[i].getPID())
          processIndex = i;
        if(currProcess_.getChildren()[childIterator] == processes_[i].getPID()){//if this process is the child of the param process 
          if(!processes_[i].getChildren().empty()){//check if it also has children
            int childsChildIterator = 0;
            while(childsChildIterator < processes_[i].getChildren().size()){//if it does, loop through processes to find its children and add it into the param processes's children to be deleted 
                  for(int j = 0; j < processes_.size(); ++j){
                    currProcess_.addChild(processes_[j].getPID());
                  }
                }
                processes_[i].getChildren().erase(processes_[i].getChildren().begin() + i);//delete the child of the param process after all its children are added
              }
              currProcess_.removeChild(childIterator);//remove it from the vector of children
            }
          }
        }
        processes_.erase(processes_.begin() + processIndex);//delete the current process to exit from it
        runFirstProcess();
  }
 }

void CPUManager::cascadingTerminationWait(){//does the same cascading termination only for a single child and after doing that, set the running to waiting
  int zombieLocation = 0;
  for(int it = 0; it < zombies_.size(); ++it){
    if(zombies_[it].getParentPID() == currProcess_.getPID()){
      zombieLocation = it;
      break;
    }
  }
  if(zombies_.size() == 0 && currProcess_.getChildren().empty()){//if there are no children or zombie for current process leave process as running
    return;
  }
  if(zombies_.size() == 1){//if there is just one zombie child, CPU will just continue running
    for(int i = 0; i < currProcess_.getChildren().size(); ++i){//loop through the children to find it 
      if(zombies_[zombieLocation].getPID() == currProcess_.getChildren()[i]){//once found, delete the zombie child from parent
        currProcess_.removeChild(i);
        break;
      }
    }
    for(int i = 0; i < processes_.size(); ++i){//remove zombie from processes
      if(processes_[i].getPID() == zombies_[zombieLocation].getPID()){
        processes_.erase(processes_.begin() + i);
        break;
      }
    }
    zombies_.erase(zombies_.begin() + zombieLocation);//erase it from zombie process as well now and let process continue
  }
  else if(zombies_.size() > 1){///else if there is more than one zombie child
    for(int i = 0; i < currProcess_.getChildren().size(); ++i){//loop through the children to find it 
      if(zombies_[zombieLocation].getPID() == currProcess_.getChildren()[i]){//once found, delete the zombie child from parent
        currProcess_.removeChild(i);
        break;
      }
    }
    for(int i = 0; i < processes_.size(); ++i){//remove zombie from processes
      if(processes_[i].getPID() == zombies_[zombieLocation].getPID()){
        processes_.erase(processes_.begin() + i);
        break;
      }
    }
    zombies_.erase(zombies_.begin() + zombieLocation);//erase it from zombie process as well now
    currProcess_.setState(WAITING);
    runFirstProcess();
  } 
  else if(zombies_.empty() && !currProcess_.getChildren().empty()){//if there are children and no zombies then set currProcess to ready in back of ready queue
    currProcess_.setState(WAITING);
    runFirstProcess();
  }
}

void CPUManager::interrupt(){
  if(!readyQueue_.empty()){//if ready queue is not empty
  //first loop through all processes to pause the current process if the stack is not empty
  for(int i = 0; i < processes_.size(); ++i){
      if(processes_[i].getPID() == CPU_){//if the current process has matching PID to the current Process
        processes_[i].setState(READY);//pause it by settings its state to ready
        readyQueue_.push_back(processes_[i].getPID());//add it to the back of the ready queue
        break;
      }
  }
  //loop through processes again to find the process at the top of the ready queue and set it to running and adding it to 
  runFirstProcess();
  }
}

void CPUManager::wait(){
  if(CPU_ != 0){ //if CPU is running
    cascadingTerminationWait();
  }
}

void CPUManager::setCPUState(int STATE){
  CPU_ = STATE;
}