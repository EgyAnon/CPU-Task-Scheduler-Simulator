#pragma once   
#include <cstddef> 
#include <functional>
#include <atomic>
#include <mutex>
#include <iostream>

class Process{   
    private:
    //This static and atomic unsigned variable is used to give every process
    //a unique id.
    static std::atomic<size_t> _id;

    public:
    bool servedBefore{false}; //has this process been served before inside the runner?

    size_t arrival_time; //arrival time 
    size_t burst_time;   //burst time (not to be modified)
    size_t remaining_burst_time;  //remanining burst time in ms
    size_t priority; //the lower this value, the higher the priority 
    size_t pid; //the unique process id

    size_t first_response_time{0};    //time until first response in ms
    size_t finish_time{0};   //in-simulation finish time (in ms)
    
    Process(size_t arrival_time, std::size_t burst_time, std::size_t priority  = 0);
    Process() = delete; 
    
    static size_t GetNumerOfProcesses(){
        return _id.load();
    }

    //These comparators are used later to insert processes inside the process list
    static bool CompareByArrivalTime(Process* p1, Process* p2);
    static bool CompareByRemainingBurstTime(Process* p1, Process* p2);
    static bool CompareByPriority(Process* p1, Process* p2);
    static bool CompareFalse(Process* p1, Process* p2);
    
    //reset the static id to 0 to restart the count.
    static void resetID();
};

//This class is used to create the linked list of process pointers.
//We let the data of the node be a pointer and not the actual process
//So that the actual process objects remain accessible by their
//pid from the processVector.
class ProcessNode{
    public:
    Process* process;   //pointer to the actual process object
    ProcessNode* next;
    ProcessNode* previous;

    ProcessNode(Process* _process):
        process(_process),
        next(nullptr),
        previous(nullptr)
    {};
};

//A thread-safe doubly-linked list of pointers to objects of type Process
class ProcessList{
    private:
    std::mutex _list_mutex;
    ProcessNode* _head;
    ProcessNode* _tail;
    size_t _size;

    public:
    ProcessList() : _head(nullptr), _tail(nullptr), _size(0){};
    void AddProcess(Process* newProcess);   //default adds to tail
    void AddProcess(Process* newProcess, std::function<bool(Process*, Process*)> compareRule);
    
    Process* GetHeadProcess();
    void PushFront(Process* p);
    void PushBack(Process* p);

    bool Empty();
    void PrintList(); //for debugging
};

