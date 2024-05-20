//Eric Wu
#include "DiskManager.h"  

FileReadRequest DiskManager::getDisk(int diskNumber){
  return currRequests_[diskNumber];
}

std::vector<FileReadRequest> DiskManager::getAllCurrentRequests(){
  return currRequests_;
}

std::deque<FileReadRequest> DiskManager::getDiskQueue(int disk){
  return IOQueue_[disk];
}

void DiskManager::setNumDisks(int totalDisks){
  numDisks_ = totalDisks;
  for(int i = 0; i < numDisks_; ++i){
    std::deque<FileReadRequest> ioQueue;
    FileReadRequest request;
    IOQueue_.push_back(ioQueue);
    
    currRequests_.push_back(request);
  }
}

void DiskManager::diskReadRequest(int processID, int numDisk, std::string file){
  FileReadRequest request;//create a default request
  request.fileName = file;
  request.PID = processID;
  if(currRequests_[numDisk].PID == 0)//if there is no current process reading at the numDisk position
    currRequests_[numDisk] = request;//then set current request to it
  else//else push it back into the IO queue where the request is stored for this particular position
    IOQueue_[numDisk].push_back(request);
}

int DiskManager::completeDiskJob(int diskNum){
  int runningPID = currRequests_[diskNum].PID;
  //temporarily pause the current request at the disk
  currRequests_[diskNum].PID = 0;
  currRequests_[diskNum].fileName = "";
  if(IOQueue_[diskNum].size() != 0){//if there are requests in this disk
    currRequests_[diskNum] = IOQueue_[diskNum].front();//complete request at the front of the disk by adding it into the current requests in the position of its disknum
    IOQueue_[diskNum].pop_front();//pop request off after finishing
  }
  return runningPID;
}

bool DiskManager::checkIfDiskExists(int diskNum){
  if(diskNum > numDisks_ || diskNum < 0){
    return false;
  }
  return true;
}