//Eric Wu
#include "MemoryManager.h"

int MemoryManager::getProcessPID(int index){
    return pageTable_[index].PID;
}

void MemoryManager::setAmountOfRAM(unsigned long long amountOfRAM){
    amountOfRAM_ = amountOfRAM;
}

void MemoryManager::setPageSize(unsigned int pageSize){
    pageSize_ = pageSize;
}

void MemoryManager::createPageTable(){
    int pageTableSize = amountOfRAM_ / pageSize_;//total num pages
    for(unsigned long long i = 0; i < pageTableSize; i++){//creates page table with default values and adds them to the recentlyUsedFrames to fill up the page table
        MemoryItem frames;
        frames.frameNumber = i;
        frames.PID = 0;
        frames.pageNumber = 0;
        pageTable_.push_back(frames);
        recentlyUsedFrames_.push_back(INT_MAX);
    }
}

void MemoryManager::accessMemoryAddress(int PID, unsigned long long address)
{
    unsigned long long pageNum = address / pageSize_;//page num of process
    bool unusedFrame = false;//if this frame has never been used before 
    bool sameFrame = false;//used to indicate if this frame has been already used before
    for (int i = 0; i < pageTable_.size(); ++i){//loops through to check if this there is already a corresponding page
        if ((getProcessPID(i) ==  PID) && (pageTable_[i].pageNumber == pageNum)){
            sameFrame = true;//set true if there is
            frameUseCounter_++;//increment its use 
            recentlyUsedFrames_[i] = frameUseCounter_;//update recently used info
            break;
        }
    }
    if (!sameFrame){//if it is not the same frame then:
        for (int i = 0; i < pageTable_.size(); ++i){//loop through page table
            if (getProcessPID(i) == 0){//if matching process, update the info at the page table
                pageTable_[i].pageNumber = pageNum;
                pageTable_[i].PID = PID;
                frameUseCounter_++;//increment useCount
                recentlyUsedFrames_[i] = frameUseCounter_;//update recently used frame to this frame
                unusedFrame = true;//this is a new frame
                break;
            }
        }
    }
    if (!sameFrame && !unusedFrame){//if this is not same frame and is unused 
        int leastRecentlyUsed = INT_MAX;//temp variable using int max so the comparator later always go through
        for (int i = 0; i < recentlyUsedFrames_.size(); ++i){//loop through the most recently used frame to find the most recently used one for us to replace
            if (recentlyUsedFrames_[i] < leastRecentlyUsed)
                leastRecentlyUsed = recentlyUsedFrames_[i];
        }
        pageTable_[leastRecentlyUsed].pageNumber = pageNum;//update our page table at this location to have the most recently used pagenumber
        pageTable_[leastRecentlyUsed].PID = PID;//same update but for PID
        recentlyUsedFrames_[leastRecentlyUsed] = frameUseCounter_; //update recently used info
    }
}

MemoryUsage MemoryManager::getMemory()
{
    currentlyUsedFrames_.clear();//clears all the curent memory items in use for us to refresh
    for (int i = 0; i < pageTable_.size(); ++i){//loops through page table, if by the end of all memory allocation, there is still a frame in the page table, add it to the list of currently used frames.
        if (getProcessPID(i) != 0){
            currentlyUsedFrames_.push_back(pageTable_[i]);
        }
    }
    return currentlyUsedFrames_;
}

void MemoryManager::removePageTableFrame(int PID)//removes frame from table with given process
{
    for (int i = 0; i < pageTable_.size(); ++i){
        if (getProcessPID(i) == PID){
            pageTable_.erase(pageTable_.begin() + i);
        }
    }
}