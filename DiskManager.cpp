#include "DiskManager.h"

DiskManager::DiskManager(){
  FileReadRequest noJobFile{0, ""};
  Process noJobProcess(0, NO_PROCESS);
  currJob.first = noJobFile;
  currJob.second = noJobProcess;
}

FileReadRequest DiskManager::getCurrentRR(){
  return currJob.first;
}

Process DiskManager::getCurrentProcess() const{
  return currJob.second;
}

std::pair<FileReadRequest, Process> DiskManager::getCurrentJob() const{
  return currJob;
}

std::deque<FileReadRequest> DiskManager::getWaitingRR(){
  std::deque<FileReadRequest> waitingFileReadRequests;
  for(auto i = diskQueue.begin(); i != diskQueue.end(); i++)
  {
      waitingFileReadRequests.push_back(i->first);
  }
  return waitingFileReadRequests;
}

std::deque<std::pair<FileReadRequest,Process>> DiskManager::getDiskQueue() const{
  return diskQueue;
}

void DiskManager::setDiskQueue(const std::deque<std::pair<FileReadRequest,Process>>& diskQ){
  diskQueue = diskQ;
}
    
void DiskManager::setCurrentProcess(const Process& process){
  currJob.second = process;
}

void DiskManager::setCurrentJob(const std::pair<FileReadRequest,Process>& job){
  currJob = job;
}
    
void DiskManager::setCurrentRR(const FileReadRequest& fileReadRequest){
  currJob.first = fileReadRequest;
}

void DiskManager::completeWait(){
  if(!diskQueue.empty()){
    currJob = diskQueue.front();
    diskQueue.pop_front();
  }
  else
    clearCurrentJob();
}

void DiskManager::addToQueue(const std::pair<FileReadRequest, Process>& job){
  if(currJob.second.getState() == 0)
    currJob = job;
  else
    diskQueue.push_back(job);
}

void DiskManager::clearCurrentJob(){
  currJob.first.fileName = "";
  currJob.first.PID = 0;

  currJob.second.setPID(0);
  currJob.second.setState(0);
}