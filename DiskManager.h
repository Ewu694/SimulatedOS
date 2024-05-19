//Eric Wu
#ifndef DISKMANAGER_H
#define DISKMANAGER_H

#include "Process.h"
#include <iostream>
#include <vector>
#include <deque>

struct FileReadRequest{
    int PID{0};
    std::string fileName{""};
};

class DiskManager{
public:
    FileReadRequest& operator=(const FileReadRequest rhs);
    int getNumDisks();//returns number of disks
    std::vector<FileReadRequest> getAllCurrentRequests();//returns a vector of all the requests
    FileReadRequest getDisk(int disk);//returns all the disks in ioqueue
    std::deque<FileReadRequest> getDiskQueue(int disk);
    void setNumDisks(int numDisks);//sets up ioqueue and all requests
    void diskReadRequest(int processID, int numDisk, std::string file);
    int completeDiskJob(int diskNum); //returns the PID of the process that is done reading

private:
    int numDisks_;
    std::vector<FileReadRequest> currRequests_;
    std::vector<std::deque<FileReadRequest>> IOQueue_;
};
#endif
