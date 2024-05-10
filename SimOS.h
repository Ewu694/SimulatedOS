#include <iostream>
#include <deque>

struct MemoryItem{};
struct FileReadRequest{};

using MemoryUsage = std::vector<MemoryItem>;
 
constexpr int NO_PROCESS{ 0 };

class SimOS{
  public: 
    SimOS( int numberOfDisks, unsigned long long amountOfRAM, unsigned int pageSize);
    void NewProcess();
    void SimFork();
    void SimExit();
    void SimWait();
    void TimerInterrupt();
    void DiskReadRequest(int diskNumber, std::string fileName);
    void DiskJobCompleted(int diskNumber);
    void AccessMemoryAddress(unsigned long long address);
    int GetCPU();
    std::deque<int> GetReadyQueue();
    MemoryUsage GetMemory();
    FileReadRequest GetDisk(int diskNumber);
    std::deque<FileReadRequest> GetDiskQueue(int diskNumber);
  
  private:
  
};