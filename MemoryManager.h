//Eric Wu
#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <vector>
#include <deque>
#include "Process.h"

struct MemoryItem{
    unsigned long long pageNumber;
    unsigned long long frameNumber;
    int PID; // PID of the process using this frame of memory
};

using MemoryUsage = std::vector<MemoryItem>;

class MemoryManager{
public:
    unsigned long long getRAMSize() const;
    MemoryUsage getMemoryUsage() const;
    unsigned int getPageSize() const;
    unsigned long long getPageNumber(const unsigned long long &address);
    unsigned long long getNumPages();
    void setNumPages(const unsigned long long &amountOfRAM, const unsigned int &pageSize);
    void setRAMSize(const unsigned long long &ramSize); //set
    void setMemoryUsage(const MemoryUsage &memory);
    void setPageSize(const unsigned int &pageS);
    void accessAddress(const int &processID, const unsigned long long &address);
    void clearMemory(const int &processID);
private:
    std::deque<unsigned long long> usedFrames;
    unsigned long long ramSize;
    unsigned int pageSize;
    unsigned int numPages;
    MemoryUsage memory;
};
#endif