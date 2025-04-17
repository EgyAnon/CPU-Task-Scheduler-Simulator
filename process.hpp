#pragma once   

#include <cstddef> 
#include <functional>
#include <atomic>
#include <mutex>
#include <iostream>

class Process{   
    private:
    static std::atomic<size_t> id;

    public:
    bool servedBefore{false};
    size_t arrivalTime;
    size_t burstTime;
    size_t remainingBurstTime;
    size_t priority;
    size_t pid;

    size_t firstResponseTime{0};
    size_t finishTime{0};
    
    Process(size_t _arrivalTime, std::size_t _BurstTime, std::size_t _priority  = 0);
    Process() = delete;
    
    static size_t getNumerOfProcesses(){
        return id.load();
    };

    static bool compareByArrivalTime(Process* p1, Process* p2);
    static bool compareByRemainingBurstTime(Process* p1, Process* p2);
    static bool compareByPriority(Process* p1, Process* p2);
    static bool compareFalse(Process* p1, Process* p2);

    static void resetID();
};

class ProcessNode{
    public:
    Process* process;
    ProcessNode* next;
    ProcessNode* previous;

    ProcessNode(Process* _process):
        process(_process),
        next(nullptr),
        previous(nullptr)
    {};
};

class ProcessList{
    private:
    std::mutex listMutex;
    ProcessNode* head;
    ProcessNode* tail;
    size_t size;

    public:
    ProcessList() : head(nullptr), tail(nullptr), size(0){};
    void addProcess(Process* newProcess);   //default adds to tail
    void addProcess(Process* newProcess, std::function<bool(Process*, Process*)> compareRule); //functor
    Process* getHeadProcess();
    void pushFront(Process* p);
    void pushBack(Process* p);
    bool empty();
    void printList();
};

