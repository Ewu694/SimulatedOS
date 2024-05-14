#include <SimOS.h>

struct FileReadRequest{
    int PID{0};
    std::string fileName{""};
};
 
struct MemoryItem{
    unsigned long long pageNumber;
    unsigned long long frameNumber;
    int PID; // PID of the process using this frame of memory
};

SimOS::SimOS(int numberOfDisks, unsigned long long amountOfRAM, unsigned int pageSize){
  num_disks = numberOfDisks;
  total_RAM = amountOfRAM;
  page_size = pageSize;
}

void SimOS::NewProcess(){
  Process newProcess;
  newProcess.setPID(processCount);
  if(CPU.getCurrentProcess() == 0){
    newProcess.setState(RUNNING);
  }
  else if(CPU.getCurrentProcess() != 0){
    newProcess.setState(WAITING);
  }
  CPU.addToReadyQueue(newProcess.getPID());
  Parents.push_back(newProcess);
  processCount++;
}

void SimOS::SimFork(){
  Process child;  
  child.setPID(processCount);
  if(CPU.getCurrentProcess() == 0){
    child.setState(RUNNING);
  }
  else if(CPU.getCurrentProcess() != 0){
    child.setState(WAITING);
  }
  CPU.addToReadyQueue(child.getPID());
  for(int i = 0; i < Parents.size(); i++){
    if(Parents[i].getPID() == CPU.getCurrentProcess()){
      Children.insert({child, Parents[i]});
    }
  }
  processCount++;
}

void SimOS::SimExit(){
  Process terminatingProcess;

}

void SimOS::SimWait(){

}

void SimOS::TimerInterrupt(){
    CPU.addToReadyQueue(CPU.getReadyQueue().front());
    CPU.getReadyQueue().pop_front();
}

void SimOS::DiskReadRequest(int diskNumber, std::string fileName){

}

void SimOS::DiskJobCompleted(int diskNumber){
  
}

void SimOS::AccessMemoryAddress(unsigned long long address){

}

int SimOS::GetCPU(){
  return CPU.getCurrentProcess();
}

std::deque<int> SimOS::GetReadyQueue(){
  return CPU.getReadyQueue();
}

MemoryUsage SimOS::GetMemory(){
  return memory;
}

FileReadRequest SimOS::GetDisk(int diskNumber){

}

std::deque<FileReadRequest> SimOS::GetDiskQueue(int diskNumber){

}
