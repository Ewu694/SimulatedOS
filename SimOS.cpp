#include <SimOS.h>

struct FileReadRequest{
    int  PID{0};
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
  if(PIDs.empty()){
    newProcess.setPID(1);
    newProcess.setState(RUNNING);
  }
  else{
    newProcess.setPID(PIDs.size() + 1);
    newProcess.setState(WAITING);
  }
  CPU.AddToReadyQueue(newProcess.getPID());
  PIDs.insert({newProcess, newProcess.getPID()});
}

void SimOS::SimFork(){
  Process child;
  child.setPID(CPU.getCurrentProcess() * 2);
  child.setState(WAITING);
  CPU.AddToReadyQueue(child.getPID());
  PIDs.insert({child, child.getPID()});
}

void SimOS::SimExit(){
  Process terminatingProcess;
  for(auto it = PIDs.begin(); it != PIDs.end(); ++it){
    if((it->second == (CPU.getCurrentProcess() / 2 == 0)) && it->first.getState() == WAITING)
    { 
      
    }
    if (it->second == CPU.getCurrentProcess())
      terminatingProcess = it->first;
  PIDs.erase(terminatingProcess);
  }
}

void SimOS::SimWait(){

}

void SimOS::TimerInterrupt(){
  
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

}

FileReadRequest SimOS::GetDisk(int diskNumber){

}

std::deque<FileReadRequest> SimOS::GetDiskQueue(int diskNumber){

}
