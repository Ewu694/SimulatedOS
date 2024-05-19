//Eric Wu
#include "SimOS.h"

SimOS::SimOS(int numberOfDisks, unsigned long long amountOfRAM, unsigned int pageSize){
  RAM.setRAMSize(amountOfRAM);
  RAM.setPageSize(pageSize);
  RAM.setNumPages(amountOfRAM, pageSize);
  disks.setNumDisks(numberOfDisks);
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
  disks.diskReadRequest(CPU.getCPUProcess(), diskNumber, fileName);
  CPU.setCPUState(0);
}

void SimOS::DiskJobCompleted(int diskNumber){
  int completedRequest = disks.completeDiskJob(diskNumber);//holds PID of completed request
  if(completedRequest != 0)
    CPU.addToReadyQueue(completedRequest);
}

void SimOS::AccessMemoryAddress(unsigned long long address){
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
  return disks.getDisk(diskNumber);
}

std::deque<FileReadRequest> SimOS::GetDiskQueue(int diskNumber){
  return disks.getDiskQueue(diskNumber);
}
