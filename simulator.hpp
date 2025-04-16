#pragma once
#include <vector>
#include <tuple>
#include <mutex>
#include <condition_variable>

#include "scheduler.hpp"
#include "runner.hpp"
#include "process.hpp"


class Simulator {
    private: 
    /*  Main Actors */
    Scheduler* scheduler = nullptr;
    Runner* runner = nullptr;
    
    /* Main Data Structures*/
    std::queue<Process*> scheduleQueue;
    ProcessList processList;
    
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
    
    std::chrono::system_clock::time_point startTime{std::chrono::system_clock::now()};
    void delayedAdd(Process* newProcess, size_t delay_s);
    void setRunner(std::string runnerType = "Regular", size_t timeQuantum = 1);
    void displayStatistics();
    void displayExecutionLog();
    void addProcess(Process p);
    
    public:
    std::vector<Process> processVector;
    std::vector<std::tuple<size_t, size_t, size_t>> executionLog;

    Simulator(){processVector.reserve(100);};
    void addProcess(size_t arrivalTime, size_t burstTime, size_t priority);
    
    void setScheduler(std::string schedulerName, size_t timeQuantum = 1);
    void start();

};
    