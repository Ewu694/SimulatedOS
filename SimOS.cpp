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
  /*if(it->second.getPID() == currProcess.getPID() && Processes[i].getPID() == it->first.getPID()){ //checks if the current process is the parent and loops through processes to terminate its children
        Processes.erase(Processes.begin() + i);
    }*/
}

void SimOS::TimerInterrupt(){
  CPU.interrupt();
}

void SimOS::DiskReadRequest(int diskNumber, std::string fileName){

}

void SimOS::DiskJobCompleted(int diskNumber){

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

//FileReadRequest SimOS::GetDisk(int diskNumber){}

//std::deque<FileReadRequest> SimOS::GetDiskQueue(int diskNumber){}
