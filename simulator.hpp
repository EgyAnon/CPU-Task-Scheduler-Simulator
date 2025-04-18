#pragma once
#include "scheduler.hpp"
#include "runner.hpp"
#define MAX_SPEEDUP 60

struct CompareByArrival {
    bool operator()(Process* a, Process* b) const;
};


class Simulator {
    private: 
    /*  Main Actors */
    Scheduler* scheduler = nullptr;
    Runner* runner = nullptr;
    
    /* Main Data Structures*/
    std::queue<Process*> scheduleQueue;
    ProcessList processList;
    std::priority_queue<Process*, std::vector<Process*>, CompareByArrival> delayedQueue;
    
    /*Synchronization Primitives*/
    std::condition_variable schedulerCV;
    std::condition_variable runnerCV;
    std::condition_variable queueCV;
    std::condition_variable delayedAddCV;
    
    std::mutex processVectorMtx;
    std::mutex scheduleQueueMtx;
    std::mutex runnerMtx;
    std::mutex schedulerMtx;

    std::atomic<bool> simulationRunning{false}, runnerFinished{false}, runnerPreempted{true}, preemptionRequest {false};
    std::atomic<size_t> delayedProcesses{0};

    //Speedup
    size_t Speedup = 1;
    
    void delayedAdd(Process* newProcess, size_t delay_s);
    void addDelayedProcesses();
    void setRunner(std::string runnerType = "Regular", size_t timeQuantum = 1);
    void displayStatistics();
    void displayExecutionLog();
    
    public:
    std::vector<Process> processVector;
    std::vector<std::tuple<size_t, size_t, size_t>> executionLog;
    Timer simulationTimer;
    
    Simulator(){processVector.reserve(100);};
    void addProcess(size_t arrivalTime, size_t burstTime, size_t priority = 0);
    void addProcessDynamically(size_t burstTime, size_t priority = 0);
    void setScheduler(std::string schedulerName, size_t timeQuantum = 1);
    void setSpeedup(size_t SPEEDUP = 1);
    void start();
    
    void printExecutionLog();
};
    
