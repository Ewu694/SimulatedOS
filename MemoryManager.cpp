#include "MemoryManager.h"


MemoryManager::MemoryManager() = default;

MemoryManager::MemoryManager(unsigned long long ramSize, unsigned int pageS){
    ramSize = ramSize;
    pageSize = pageS;
    numPages = ramSize/pageS;
    MemoryItem temp{0, 0, 0};
    for(unsigned int i = 0; i < numPages; i++){  
        usedFrames.push_back(i);
    }
}

void MemoryManager::setRAMSize(unsigned long long ramSize)
{
    ramSize = ramSize;
}

void MemoryManager::setPageSize(unsigned int pageS)
{
    pageSize = pageS;
}

void MemoryManager::setMemoryUsage(MemoryUsage memoryU)
{
    memory = memoryU;
}

unsigned long long MemoryManager::getRAMSize()
{
    return ramSize;
}

unsigned int MemoryManager::getPageSize() 
{
    return pageSize;
}

MemoryUsage MemoryManager::getMemoryUsage() 
{
    return memory;
}

unsigned long long MemoryManager::getPageNumber(unsigned long long address)
{
    return address / pageSize;
}

void MemoryManager::accessMemoryAtAddress(int processID, unsigned long long address)
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

void MemoryManager::findAndClearMemoryUsedByAProcess(int processID)
{
    for(auto i = memory.begin(); i != memory.end(); ++i)
    {
        if(i->PID == processID)
            i = memory.erase(i);
    }
}