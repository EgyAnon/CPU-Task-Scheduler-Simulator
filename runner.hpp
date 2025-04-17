#pragma once

#include "process.hpp"
#include <condition_variable>
#include <mutex>
#include <vector>
#include <tuple>
#include <atomic>
#include <chrono>
#include <thread>
#include <iostream>

/**
 * Responsible for executing processes.
 * Executes one process at a time from the processList and handles preemption.
 */
class Runner {
    protected:
    /*  Data Structures */
    ProcessList* processList;
    std::vector<std::tuple<size_t, size_t, size_t>>* executionLog;
    
    /*Synchronization Primitives*/
    std::mutex *runnerMtx;
    std::mutex *schedulerMtx;
    
    std::condition_variable *schedulerCV;
    std::condition_variable *runnerCV;
    
    /* Signals */
    std::atomic<size_t>* delayedProcesses;
    std::atomic<bool> *runnerFinished, *runnerPreempted, *preemptionRequest;
    
    /* Timing */
    std::chrono::system_clock::time_point* startTime;
    
    /* Runner specific*/
    Process* runningProcess = nullptr;
    
    /*funcitons*/
    inline size_t getCurrentTime();
    
    public:
    Runner(
        ProcessList* _processList,
        std::vector<std::tuple<size_t, size_t, size_t>>* _executionLog,
        
        std::mutex* _runnerMtx,
        std::mutex* _schedulerMtx,
        std::condition_variable* _schedulerCV,
        std::condition_variable* _runnerCV,

        std::atomic<size_t>* _delayedProcesses,
        std::atomic<bool>* _runnerFinished,
        std::atomic<bool>* _runnerPreempted, std::atomic<bool>* _preemptionRequest,

        std::chrono::system_clock::time_point* _startTime
    ):
    processList(_processList), executionLog(_executionLog),
    runnerMtx(_runnerMtx), schedulerMtx(_schedulerMtx), schedulerCV(_schedulerCV), runnerCV(_runnerCV),
    delayedProcesses(_delayedProcesses),
    runnerFinished(_runnerFinished), runnerPreempted(_runnerPreempted),
    preemptionRequest(_preemptionRequest), startTime(_startTime)
    {};
    
    virtual void run();
};

class Runner_RR : public Runner{
    size_t timeQuantum;
    
    public: 
    Runner_RR(
        ProcessList* _processList,
        std::vector<std::tuple<size_t, size_t, size_t>>* _executionLog,

        std::mutex* _runnerMtx,
        std::mutex* _schedulerMtx,
        std::condition_variable* _schedulerCV,
        std::condition_variable* _runnerCV,

        std::atomic<size_t>* _delayedProcesses,
        std::atomic<bool>* _runnerFinished,
        std::atomic<bool>* _runnerPreempted, std::atomic<bool>* _preemptionRequest,

        std::chrono::system_clock::time_point* _startTime,
        size_t _timeQuantum
    ):
    Runner(
        _processList, _executionLog,
        _runnerMtx, _schedulerMtx, _schedulerCV, _runnerCV,
        _delayedProcesses, _runnerFinished, _runnerPreempted, _preemptionRequest,
        _startTime
    ),
    timeQuantum(_timeQuantum)
    {};
    
    void run() override;
    void setTimeQuantum(size_t _timeQuantum);
};