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
    MemoryManager(const unsigned long long& ramSize, const unsigned int& pageS);
    void setRAMSize(const unsigned long long& ramSize);
    void setPageSize(const unsigned int& pageS);
    void setMemoryUsage(const MemoryUsage& memory);
    unsigned long long getRAMSize() const;
    unsigned int getPageSize() const;
    MemoryUsage getMemoryUsage() const;
    unsigned long long getPageNumber(const unsigned long long& address);
    void accessMemoryAtAddress(const int& processID, const unsigned long long& address);
    void findAndClearMemoryUsedByAProcess(const int& processID);

private:
    std::deque<unsigned long long> usedFrames;
    unsigned long long ramSize;
    unsigned int pageSize;
    unsigned int numPages;
    MemoryUsage memory;
};
#endif