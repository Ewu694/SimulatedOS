#include "SimOS.h"

SimOS::SimOS() = default;

SimOS::SimOS(int numberOfDisks, unsigned long long amountOfRAM, unsigned int pageSize){
  num_disks = numberOfDisks;
  total_RAM = amountOfRAM;
  page_size = pageSize;
}

void SimOS::NewProcess(){
  CPU.createProcess();
}

void SimOS::SimFork(){
  CPU.forkProcess();
}

void SimOS::SimExit(){
  RAM.clearMemory(CPU.getCPUProcess());  
  CPU.exitProcess();
}

void SimOS::SimWait(){
  CPU.wait();
}

void SimOS::TimerInterrupt(){
  CPU.interrupt();
}

void SimOS::DiskReadRequest(int diskNumber, std::string fileName){
  if(diskNumber < disks.size() && diskNumber > 0){
    FileReadRequest newRequest{CPU.getCPUProcess(), fileName};
    std::pair<FileReadRequest, Process> job{newRequest, CPU.getCurrentProcess()};
    disks[diskNumber].addToQueue(job);
    CPU.runFirstProcess();
  }
}

void SimOS::DiskJobCompleted(int diskNumber){
  if(diskNumber < disks.size()){
    Process process = disks[diskNumber].getCurrentProcess();
    CPU.addToReadyQueue(process);
    disks[diskNumber].clearCurrentJob();
  }
}

void SimOS::AccessMemoryAddress(unsigned long long address){
  if(CPU.getCPUProcess() == 0)
    RAM.accessAddress(CPU.getCPUProcess(), address);
}

int SimOS::GetCPU(){
  return CPU.getCPUProcess();
}

std::deque<int> SimOS::GetReadyQueue(){
  return CPU.getReadyQueue();
}

MemoryUsage SimOS::GetMemory(){
  return RAM.getMemoryUsage();
}

FileReadRequest SimOS::GetDisk(int diskNumber){
  if(diskNumber < disks.size())
    return disks[diskNumber].getCurrentRR();
  else
    throw std::out_of_range("The disk with the requested number does not exist");
}

std::deque<FileReadRequest> SimOS::GetDiskQueue(int diskNumber){
  if(diskNumber < disks.size())
    return disks[diskNumber].getWaitingRR();
  else 
    throw std::out_of_range("The disk with the requested number does not exist");
}
