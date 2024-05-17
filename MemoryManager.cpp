#include "MemoryManager.h"


MemoryManager::MemoryManager() = default;

MemoryManager::MemoryManager(const unsigned long long& ram, const unsigned int& pageS){
    ramSize = ram;
    pageSize = pageS;
    numPages = ramSize/pageS;
    MemoryItem temp{0, 0, 0};
    for(unsigned int i = 0; i < numPages; i++){  
        usedFrames.push_back(i);
    }
}

void MemoryManager::setRAMSize(const unsigned long long& ram)
{
    ramSize = ram;
}

void MemoryManager::setPageSize(const unsigned int& pageS)
{
    pageSize = pageS;
}

void MemoryManager::setMemoryUsage(const MemoryUsage& memoryU)
{
    memory = memoryU;
}

unsigned long long MemoryManager::getRAMSize() const
{
    return ramSize;
}

unsigned int MemoryManager::getPageSize() const
{
    return pageSize;
}

MemoryUsage MemoryManager::getMemoryUsage() const
{
    return memory;
}

unsigned long long MemoryManager::getPageNumber(const unsigned long long& address)
{
    return address / pageSize;
}

void MemoryManager::accessMemoryAtAddress(const int& processID, const unsigned long long& address)
{
    MemoryItem memoryAccess; //initialize temp memory access to prep 
    memoryAccess.pageNumber = address / pageSize;
    memoryAccess.frameNumber = usedFrames.front();
    memoryAccess.PID = processID;

    usedFrames.push_back(memoryAccess.frameNumber); //pushes most recently used frame
    usedFrames.pop_front(); //pops recently used frame

    //Check to see if the frame we're about to use is currently being used, and if so, replace its page with the new page
    bool frameInUse = false;
    for(auto i = memory.begin(); i != memory.end(); ++i) //parses through memory to check if the frame is currently being used
    {
        if(i->frameNumber == memoryAccess.frameNumber){
            frameInUse = true;
            *i = memoryAccess; //replaces 
            break;
        }
    }
    if(!frameInUse) //after parsing through, if the frame is not being used, push it back to memory
        memory.push_back(memoryAccess);
}

void MemoryManager::findAndClearMemoryUsedByAProcess(const int& processID)
{
    for(auto i = memory.begin(); i != memory.end(); ++i)
    {
        if(i->PID == processID)
            i = memory.erase(i);
    }
}