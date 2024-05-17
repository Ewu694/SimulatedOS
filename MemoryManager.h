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
    MemoryManager(unsigned long long ramSize, unsigned int pageS);
    void setRAMSize(unsigned long long ramSize);
    void setPageSize(unsigned int pageS);
    void setMemoryUsage(MemoryUsage memory);
    unsigned long long getRAMSize();
    unsigned int getPageSize();
    MemoryUsage getMemoryUsage();
    unsigned long long getPageNumber(unsigned long long address);
    void accessMemoryAtAddress(int processID, unsigned long long address);
    void findAndClearMemoryUsedByAProcess(int processID);

private:
    std::deque<unsigned long long> usedFrames;
    unsigned long long ramSize;
    unsigned int pageSize;
    unsigned int numPages;
    MemoryUsage memory;
};
#endif