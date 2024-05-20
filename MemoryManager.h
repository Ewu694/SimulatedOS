//Eric Wu

#ifndef MEMORY_MANAGER
#define MEMORY_MANAGER

#include <vector>
#include <limits.h>

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
        int getProcessPID(int index);
        unsigned long long getProcessPageNumber(int index);
        void setAmountOfRAM(unsigned long long amountOfRAM);
        void setPageSize(unsigned int pageSize);
        void createPageTable();
        void accessMemoryAddress(int PID, unsigned long long address);
        MemoryUsage getMemory();
        void removePageTableFrame(int PID);
    private:
        unsigned long long amountOfRAM_ = 0;
        unsigned int pageSize_ = 0;
        MemoryUsage pageTable_;
        int frameUseCounter_ = 0;
        std::vector<int> recentlyUsedFrames_;
        MemoryUsage currentlyUsedFrames_;
};


#endif