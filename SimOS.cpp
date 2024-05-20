//Eric Wu
#include "SimOS.h"

SimOS::SimOS(int numberOfDisks, unsigned long long amountOfRAM, unsigned int pageSize){
    numberOfDisks_ = numberOfDisks;
    amountOfRam_ = amountOfRAM;
    pageSize_ = pageSize;
    RAM_.setAmountOfRAM(amountOfRAM);
    RAM_.setPageSize(pageSize);
    RAM_.createPageTable();
    disks_.setNumDisks(numberOfDisks);
}

void SimOS::NewProcess(){
  CPU_.createProcess();

}

void SimOS::SimFork(){
  if(CPU_.getCPUProcess() != 0)
    CPU_.forkProcess();
  else
    throw std::logic_error("CPU is idle");
}

void SimOS::SimExit(){
  if(CPU_.getCPUProcess() != 0){
    RAM_.removePageTableFrame(CPU_.getCPUProcess());
    CPU_.exitProcess();
  }
  else
    throw std::logic_error("CPU is idle");
}

void SimOS::SimWait(){
  if(CPU_.getCPUProcess() != 0)
    CPU_.wait();
  else
    throw std::logic_error("CPU is idle");
}

void SimOS::TimerInterrupt(){
  if(CPU_.getCPUProcess() != 0)
    CPU_.interrupt();
  else
    throw std::logic_error("CPU is idle");
}

void SimOS::DiskReadRequest(int diskNumber, std::string fileName){
  disks_.diskReadRequest(CPU_.getCPUProcess(), diskNumber, fileName);
  CPU_.setCPUState(0);
}

void SimOS::DiskJobCompleted(int diskNumber){
  if(disks_.checkIfDiskExists(diskNumber)){
    int completedRequest = disks_.completeDiskJob(diskNumber);//holds PID of completed request
    if(completedRequest != 0)
      CPU_.addToReadyQueue(completedRequest);
  }
  else{
    throw std::logic_error("Instruction is a running process");
  }
}

void SimOS::AccessMemoryAddress(unsigned long long address){
  if(CPU_.getCPUProcess() != 0)
    RAM_.accessMemoryAddress(CPU_.getCPUProcess(), address);
  else
    throw std::logic_error("CPU is idle");
}

int SimOS::GetCPU(){
  return CPU_.getCPUProcess();
}

std::deque<int> SimOS::GetReadyQueue(){
  return CPU_.getReadyQueue();
}

MemoryUsage SimOS::GetMemory(){
  return RAM_.getMemory();
}

FileReadRequest SimOS::GetDisk(int diskNumber){
  if(disks_.checkIfDiskExists(diskNumber))
    return disks_.getDisk(diskNumber);
  else
    throw std::logic_error("Instruction is a running process");
}

std::deque<FileReadRequest> SimOS::GetDiskQueue(int diskNumber){
  if(disks_.checkIfDiskExists(diskNumber))
    return disks_.getDiskQueue(diskNumber);
  else
    throw std::logic_error("Instruction is a running process");
}
