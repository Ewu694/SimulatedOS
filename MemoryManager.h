#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <vector>
#include <deque>
#include "Process.h"

struct MemoryItem
{
    unsigned long long pageNumber;
    unsigned long long frameNumber;
    int PID; // PID of the process using this frame of memory
};

using MemoryUsage = std::vector<MemoryItem>;

class MemoryManager
{
public:
    MemoryManager();
    MemoryManager(unsigned long long ramSize,unsigned int pageS);
    void setRAMSize(unsigned long long ramSize);
    void setMemoryUsage(MemoryUsage memory);
    void setPageSize( unsigned int pageS);
    unsigned long long getRAMSize();
    MemoryUsage getMemoryUsage();
    unsigned int getPageSize();
    unsigned long long getPageNumber( unsigned long long address);
    void accessAddress(int processID,  unsigned long long address);
    void clearMemory( int processID);

private:
    std::deque<unsigned long long> usedFrames;
    unsigned long long ramSize;
    unsigned int pageSize;
    unsigned int numPages;
    MemoryUsage memory;
};
#endif