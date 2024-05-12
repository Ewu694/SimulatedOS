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

}

void SimOS::SimFork(){
    
}

void SimOS::SimExit(){

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
