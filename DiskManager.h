#ifndef DISKMANAGER_H
#define DISKMANAGER_H

#include "Process.h"
#include <iostream>
#include <deque>

struct FileReadRequest{
    int PID{0};
    std::string fileName{""};
};

class DiskManager{
public:
    DiskManager(); //default param
    FileReadRequest getCurrentRR(); //returns the curent file read request of the current job 
    Process getCurrentProcess() const; //returns current process in disk manager
    std::pair<FileReadRequest, Process> getCurrentJob() const; //returns current job 
    std::deque<FileReadRequest> getWaitingRR(); //returns a deque of waiting read requests
    std::deque<std::pair<FileReadRequest,Process>> getDiskQueue() const; //returns the disk queue
    void setDiskQueue(const std::deque<std::pair<FileReadRequest,Process>>& diskQ); //sets disk queue to a given disk queue
    void setCurrentProcess(const Process& process); //sets current process of current job to that of a given one
    void setCurrentJob(const std::pair<FileReadRequest,Process>& job); //sets current job of current job to that of a given one
    void setCurrentRR(const FileReadRequest& fileReadRequest); //sets current ready request to that of a given one
    void completeWait(); //if diskqueue isnt empty, complete the read request in IO queue, if empty, clear the current job
    void addToQueue(const std::pair<FileReadRequest, Process>& job); //job is either handled immediately or is sent to the IO queue depending on whether or not theres already a job being handled
    void clearCurrentJob(); //clears current request
private:
    std::pair<FileReadRequest, Process> currJob;
    std::deque<std::pair <FileReadRequest, Process>> diskQueue;
};
#endif