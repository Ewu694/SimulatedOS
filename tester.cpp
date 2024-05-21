#include "SimOS.h"

void printQueue(std::deque<int> q){
    std::cout << "Current Ready Queue: ";
    for (int i : q){
        std::cout << i << " ";
    }

    std::cout << std::endl;
}

void printQueue(int diskNumber, std::deque<FileReadRequest> q){
    std::cout << "Disk Number " << diskNumber << " Queue: ";
    for (FileReadRequest i : q){
        std::cout << i.PID << " ";
    }
    std::cout << std::endl;
}

void printDisk(SimOS &OS, int diskNumber){
    std::cout << "Disk " << diskNumber << " Currently Serving: " << OS.GetDisk(diskNumber).PID << std::endl;
}

void printMemory(MemoryUsage memory_){
    std::cout << "Current Memory Usage:\n";
    for(MemoryItem i : memory_){
        std::cout << "PID: " << i.PID << "\nPage Number: " << i.pageNumber << "\nFrame Number: " << i.frameNumber << "\n\n";
    }

    std::cout << std::endl;
}

void printRecent(MemoryUsage recentlyUsed_){
    std::cout << "Most Recent Memory Items:\n";
    for(MemoryItem i : recentlyUsed_){
        std::cout << "PID: " << i.PID << "\nPage Number: " << i.pageNumber << "\nFrame Number: " << i.frameNumber << "\n\n";
    }

    std::cout << std::endl;
}

void printMemoryItem(MemoryItem &item){
    std::cout << "PID: " << item.PID << "\nPage Number: " << item.pageNumber << "\nFrame Number: " << item.frameNumber << "\n\n";
}

void printOS(SimOS &OS){
    std::cout << "Currently Running: " << OS.GetCPU() << std::endl;
    std::deque<int> q = OS.GetReadyQueue();
    printQueue(q);
}

void queueUp(SimOS &OS, int pid){
    while(OS.GetCPU() != pid){
        OS.TimerInterrupt();
    }
}

void readAll(SimOS &OS, int pid){
    int i = 0;
    while(OS.GetCPU() != pid){
        OS.DiskReadRequest(i, "hello");
        i++;
        if(i == 5){
            i = 0;
        }
    }
}

void finishAll(SimOS &OS){
    for(int i = 0; i < 5; i++){
        OS.DiskJobCompleted(i);
    }
}

void printAllDiskQueues(SimOS &OS){
    for(int i = 0; i < 5; i++){
        printDisk(OS, i);
        printQueue(i, OS.GetDiskQueue(i));
    }
}

void exitAll(SimOS &OS){
    for(int i = 0; i <= OS.GetReadyQueue().size(); i++){
        OS.SimExit();
    }
}

bool testCurrentProcess(SimOS &OS, int correctPID){
    if(OS.GetCPU() != correctPID){
        std::cout << "Error: Running process is " << OS.GetCPU() << " when it should be " << correctPID << "\n\n";
        return false;
    }

    return true;
}

bool testReadyQueue(SimOS &OS, std::string correct_1, std::string correct_2){
    std::string readyQueue = "";

    for(int i : OS.GetReadyQueue()){
        readyQueue.append(std::to_string(i));
        readyQueue += " ";
    }

    if(!(readyQueue == correct_1 || readyQueue == correct_2)){
        std::cout << "Error: Current ready queue is: " << readyQueue << "\n";
        std::cout << "It should be either " << correct_1 << "or " << correct_2 << "\n\n";
        return false;
    }
    
    return true;
}

bool testDiskProcess(SimOS &OS, int diskNumber, int correctPID, std::string correctFileName){
    bool result = true;
    if(OS.GetDisk(diskNumber).PID != correctPID){
        std::cout << "Error: Current FileReadRequest being served by disk " << diskNumber << " has a PID of " << OS.GetDisk(diskNumber).PID << " when it should be " << correctPID << "\n\n";
        result = false;
    }

    if (OS.GetDisk(diskNumber).fileName != correctFileName){
        std::cout << "Error: Current FileReadRequest being served by disk " << diskNumber << " has a file name of " << OS.GetDisk(diskNumber).fileName << " when it should be " << correctFileName << "\n\n";
        result = false;
    }

    return result;
}

bool testDiskQueue(SimOS &OS, int diskNumber, std::string correct_1, std::string correct_2){
    std::string diskQueue;

    for(FileReadRequest i : OS.GetDiskQueue(diskNumber)){
        diskQueue.append(std::to_string(i.PID));
        diskQueue += " ";
    }

    if(!(diskQueue == correct_1 || diskQueue == correct_2)){
        std::cout << "Error: Current disk queue is: " << diskQueue << "\n";
        std::cout << "It should be either " << correct_1 << "or " << correct_2 << "\n\n";
        return false;
    }
    return true;
}

bool testFrame(SimOS &OS, int correctPID, int correctPageNumber, int correctFrameNumber){
    bool result = true;
    MemoryItem test = OS.GetMemory()[correctFrameNumber];

    if(test.PID != correctPID){
        std::cout << "Error: Element " << correctFrameNumber << "of MemoryUsage item has PID " << test.PID << " when it should be " << correctPID << "\n";
        result = false;
    }

    if(test.pageNumber != correctPageNumber){
        std::cout << "Error: Element " << correctFrameNumber << "of MemoryUsage item has page number " << test.pageNumber << " when it should be " << correctPageNumber << "\n";
        result = false;
    }

    if(test.frameNumber != correctFrameNumber){
        std::cout << "Error: Element " << correctFrameNumber << "of MemoryUsage item has PID " << test.frameNumber << " when it should be " << correctFrameNumber << "\n";
        result = false;
    }

    return result;
}

bool testReadyQueueErasure(SimOS &OS, int pid){
    for(int i : OS.GetReadyQueue()){
        if (i == pid){
            std::cout << "Error: Process " << pid << " is sill in ready queue despite its parent calling SimExit().\n";
            return false;
        }
    }
    return true;
}

bool testDiskErasure(SimOS &OS, int pid, int diskNumber){
    bool result = true;

    if(OS.GetDisk(diskNumber).PID == pid){
        std::cout << "Error: Process " << pid << " is still being served by disk " << diskNumber << " despite its parent calling SimExit().\n";
        result = false;
    }

    for(FileReadRequest i : OS.GetDiskQueue(diskNumber)){
        if(i.PID == pid){
            std::cout << "Error: Process " << pid << " is sill in disk " << diskNumber << "'s disk queue despite its parent calling SimExit().\n";
            result = false;
            break;
        }
    }

    return result;
}

bool testMemoryErasure(SimOS &OS, int pid){
    for(MemoryItem i : OS.GetMemory()){
        if(i.PID == pid){
            std::cout << "Error: Process " << pid << " still occupying memory despite either itself or its parent calling SimExit().\n";
            return false;
        }
    }

    return true;
}

int main(){

    //CPU Scheduling
        SimOS OS_1(5, 1000, 100);

        //Test NewProcess()
        OS_1.NewProcess();
        OS_1.NewProcess(); 
        OS_1.NewProcess();
        OS_1.NewProcess();

        if(!testCurrentProcess(OS_1, 1) || !testReadyQueue(OS_1, "2 3 4 ", "4 3 2 ")){
            std::cout << "Failed Test 1/8: NewProcess()\n";
            return 0;
        }

        //Test TimerInterrupt()
        queueUp(OS_1, 2);
        if(!testCurrentProcess(OS_1, 2) || !testReadyQueue(OS_1, "3 4 1 ", "1 4 3 ")){
            std::cout << "Failed Test 2/8: TimerInterrupt()\n";
            return 0;
        }
        queueUp(OS_1, 1);


        //Test SimFork()
        OS_1.SimFork();
        OS_1.SimFork();

        if(!testCurrentProcess(OS_1, 1) || !testReadyQueue(OS_1, "2 3 4 5 6 ", "6 5 4 3 2 ")){
            std::cout << "Failed Test 3/8: SimFork()\n";
            return 0;
        }
        
        //Test SimWait()
        OS_1.SimWait();
        if(!testCurrentProcess(OS_1, 2) || !testReadyQueue(OS_1, "3 4 5 6 ", "6 5 4 3 ")){
            std::cout << "Failed Test 4.33/8: SimWait()\n";
            return 0;
        }

        queueUp(OS_1, 5);
        
            //First SimExit() Test
            OS_1.SimExit();
            if(!testCurrentProcess(OS_1, 6) || !testReadyQueue(OS_1, "2 3 4 1 ", "1 4 3 2 ")){
                std::cout << "Failed Test 4.66/8: SimExit() on a child process does not bring back its waiting parent process to the ready queue\n";
                return 0;
            }

        OS_1.SimExit();
        queueUp(OS_1, 1);
        OS_1.SimWait();

        if(!testCurrentProcess(OS_1, 1) || !testReadyQueue(OS_1, "2 3 4 ", "4 3 2 ")){
            std::cout << "Failed Test 4.99/8: SimWait() on a process that has a zombie child does not continue running on the CPU\n";
            return 0;
        }

    //Test Disk Management

        //Test DiskReadRequest()
        OS_1.DiskReadRequest(0, "Jayf64.txt");
        OS_1.DiskReadRequest(0, "Jayf64.txt");

        if(!testCurrentProcess(OS_1, 3) || !testReadyQueue(OS_1, "4 ", "4 ")){
            std::cout << "Failed Test 5/8: DiskReadRequest() should result in the currently running process leaving the CPU, and the ready queue should adjust accordingly\n";
            return 0;
        }

        if(!testDiskProcess(OS_1, 0, 1, "Jayf64.txt") || !testDiskQueue(OS_1, 0, "2 ", "2 ")){
            std::cout << "Failed Test 5/8: DiskReadRequest()\n";
            return 0;
        }

        //Test DiskJobComplete()
        OS_1.DiskJobCompleted(0);

        if(!testReadyQueue(OS_1, "4 1 ", "1 4 ")){
            std::cout << "Failed Test 6.33/8: DiskJobCompleted() should result in the currently served process of a disk being pushed back into the ready queue\n";
            return 0;
        }

        if(!testDiskProcess(OS_1, 0, 2, "Jayf64.txt") || !testDiskQueue(OS_1, 0, "", "")){
            std::cout << "Failed Test 6.66/8: DiskJobCompleted()\n";
            return 0;
        }

        OS_1.DiskJobCompleted(0);
        if(!testDiskProcess(OS_1, 0, 0, "")){
            std::cout << "Failed Test 6.99/8: DiskJobCompleted() should leave the FileReadRequest of a disk as a default FileReadRequest where PID = 0 and FileName is an empty string\n";
            return 0;
        }

        queueUp(OS_1, 1);

    //Test Memory Management

        //Test AccessMemoryAddress()
        OS_1.AccessMemoryAddress(0);
        if(!testFrame(OS_1, 1, 0, 0)){
            std::cout << "Failed Test 7.1/8: AccessMemoryAddress()\n";
            return 0;
        }

        OS_1.AccessMemoryAddress(50);
        if(!testFrame(OS_1, 1, 0, 0)){
            std::cout << "Failed Test 7.2/8: AccessMemoryAddress() changed existing frame when it shouldn't have\n";
            return 0;
        }

        for(int i = 1; i < OS_1.GetMemory().size(); i++){
            if(!testFrame(OS_1, 0, 0, i)){
                std::cout << "Failed Test 7.3/8: If an address is accessed and a MemoryItem with the same PID and pageNumber already exists in MemoryUsage, then nothing new happens.\n";
                std::cout << "Also, keep in mind that accessing the same page number by the same process should result in that specific MemoryItem being the most recently accessed item.\n";
                std::cout << "This will be important when MemoryUsage is full and you want to make a new entry.";
                std::cout << "Here is what your MemoryUsage looks like: \n";
                printMemory(OS_1.GetMemory());
                return 0;
            }
        }

        for(int i = 0; i < 1000; i += 100){
            OS_1.AccessMemoryAddress(i);
        }
            
        for(int i = 0; i < OS_1.GetMemory().size(); i++){
            if(!testFrame(OS_1, 1, i, i)){
                std::cout << "Failed Test 7.4/8: MemoryUsage wasn't filled properly. MemoryItems should all have a PID of 1, pageNumbers 0-9, frameNumbers 0-9\n";
                std::cout << "Here is what your MemoryUsage looks like: \n";
                printMemory(OS_1.GetMemory());
                return 0;
            }
        }

        OS_1.TimerInterrupt();
        OS_1.AccessMemoryAddress(0);

        if(!testFrame(OS_1, 2, 0, 0)){
            std::cout << "Failed Test 7.5/8: When you call AccessMemoryAddress(), MemoryUsage is full, and the MemoryItem entry you need is not present, the least recently accessed MemoryItem ";
            std::cout << "be removed from MemoryUsage. The new MemoryItem should then be placed in the same spot that the item you just removed was at, and this new item should be the most recently accessed item.\n";
            MemoryItem item;
            item.PID = 2;
            item.pageNumber = 0;
            item.frameNumber = 0;
            std::cout << "Here's what your MemoryUsage looks like: \n";
            printMemory(OS_1.GetMemory());
        }
    
    //Test SimExit()
    
    OS_1.SimFork(); //2 -> 7
    OS_1.SimFork(); //2 -> 8
    

    OS_1.TimerInterrupt();
    
    OS_1.SimFork(); //3 -> 9

    queueUp(OS_1, 1);
    OS_1.SimFork();   //1 -> 10

    //printOS(OS_1);
    OS_1.TimerInterrupt();
    OS_1.AccessMemoryAddress(100);  //Frame 1 -> PID = 7

    OS_1.TimerInterrupt();
    OS_1.AccessMemoryAddress(200); //Frame 2 -> PID = 8

    OS_1.TimerInterrupt();
    OS_1.AccessMemoryAddress(0);    //Frame 0 is recently used now

    OS_1.TimerInterrupt();
    OS_1.DiskReadRequest(0, "FollowJayf64onTwitch.txt");

    OS_1.DiskReadRequest(0, "hi.txt");
    OS_1.DiskReadRequest(3, "hi.txt");
    OS_1.DiskReadRequest(2, "hi.txt");

    OS_1.SimExit();
    if(!testMemoryErasure(OS_1, 1)){
        std::cout << "Failed Test 8/8: SimExit() on process 1 did not erase it from Memory.\n";
        std::cout << "Here's what your MemoryUsage looks like: ";
        printMemory(OS_1.GetMemory());
        return 0;
    }

    if(!testDiskErasure(OS_1, 10, 2)){
        std::cout << "Failed Test 8/8: SimExit() on process 1 did not make disk 2 stop serving process 10.\n";
        return 0;
    }

    queueUp(OS_1, 2);
    OS_1.SimExit();

    if(!testMemoryErasure(OS_1, 2)){
        std::cout << "Failed Test 8/8: SimExit() on process 2 did not erase it from Memory.\n";
        std::cout << "Here's what your MemoryUsage looks like: ";
        printMemory(OS_1.GetMemory());
        return 0;
    }

    if(!testMemoryErasure(OS_1, 7)){
        std::cout << "Failed Test 8/8: SimExit() on process 2 did not erase process 7 from Memory.\n";
        std::cout << "Here's what your MemoryUsage looks like: ";
        printMemory(OS_1.GetMemory());
        return 0;
    }

    if(!testMemoryErasure(OS_1, 8)){
        std::cout << "Failed Test 8/8: SimExit() on process 2 did not erase process 8 from Memory.\n";
        std::cout << "Here's what your MemoryUsage looks like: ";
        printMemory(OS_1.GetMemory());
        return 0;
    }

    OS_1.DiskJobCompleted(0);
    OS_1.DiskJobCompleted(0);
    OS_1.SimFork(); //9 -> 11
    OS_1.DiskReadRequest(0, "AlmostThere.txt");
    OS_1.NewProcess(); //12
    
    queueUp(OS_1, 12);
    OS_1.SimFork();    //12 -> 13
    queueUp(OS_1, 13);
    
    OS_1.DiskReadRequest(0, "AlmostThere.txt");
    
    queueUp(OS_1, 11);
    OS_1.AccessMemoryAddress(100);
    OS_1.DiskReadRequest(0, "AlmostThere.txt");

    OS_1.SimExit();

    if(!testDiskErasure(OS_1, 13, 0)){
        std::cout << "Failed Test 8/8: SimExit() on process 12 did not erase process 13 from disk queue 0.\n";
        return 0;
    }

    OS_1.SimExit();

    if(!testDiskErasure(OS_1, 9, 0)){
        std::cout << "Failed Test 8/8: SimExit() on process 3 did not make disk 0 stop serving process 9.\n";
        return 0;
    }

    if(!testDiskErasure(OS_1, 11, 0)){
        std::cout << "Failed Test 8/8: SimExit() on process 3 did not erase process 11 from disk queue 0.\n";
    }

    if(!testMemoryErasure(OS_1, 11)){
        std::cout << "Failed Test 8/8: SimExit() on process 2 did not erase process 8 from Memory.\n";
        std::cout << "Here's what your MemoryUsage looks like: ";
        printMemory(OS_1.GetMemory());
        return 0;
    }

    std::cout << "Congratulations! Your program should work perfectly. Let me know if this helped you out!\n";
    std::cout << "Follow me on YouTube or Twitch or whatever lol: Jayf64\n";
}