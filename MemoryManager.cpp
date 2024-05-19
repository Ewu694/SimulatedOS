//Eric Wu
#include "MemoryManager.h"

unsigned long long MemoryManager::getNumPages(){
    return numPages;
}

unsigned long long MemoryManager::getRAMSize() const {
    return ramSize;
}

unsigned int MemoryManager::getPageSize() const {
    return pageSize;
}

MemoryUsage MemoryManager::getMemoryUsage() const {
    return memory;
}

unsigned long long MemoryManager::getPageNumber(const unsigned long long &address) {
    return address / pageSize;
}

void MemoryManager::setRAMSize(const unsigned long long &ram) {
    ramSize = ram;
}

void MemoryManager::setPageSize(const unsigned int &pageS) {
    pageSize = pageS;
}

void MemoryManager::setMemoryUsage(const MemoryUsage &memoryU) {
    memory = memoryU;
}

void MemoryManager::setNumPages(const unsigned long long &amountOfRAM, const unsigned int &pageSize){
    numPages = ramSize / pageSize;
    for(unsigned int i = 0; i < numPages; i++) {
        usedFrames.push_back(i);
    }
}

void MemoryManager::accessAddress(const int &processID, const unsigned long long &address) {
    MemoryItem memoryAccess;
    memoryAccess.pageNumber = address / pageSize;
    memoryAccess.frameNumber = usedFrames.front();
    memoryAccess.PID = processID;

    usedFrames.push_back(memoryAccess.frameNumber);
    usedFrames.pop_front();

    bool frameInUse = false;
    for(auto it = memory.begin(); it != memory.end(); ++it) {
        if(it->frameNumber == memoryAccess.frameNumber) {
            frameInUse = true;
            *it = memoryAccess;
            break;
        }
    }
    if(!frameInUse) 
        memory.push_back(memoryAccess);
}

void MemoryManager::clearMemory(const int &processID) {
    for(auto it = memory.begin(); it != memory.end();) {
        if(it->PID == processID)
            it = memory.erase(it);
        else 
            ++it;
    }
}