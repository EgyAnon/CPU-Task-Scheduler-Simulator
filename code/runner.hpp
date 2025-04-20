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
#include "timer.hpp"

class Simulator;

class Runner {
    protected:
    /*Simulator*/
    Simulator* simulator = nullptr;

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
    std::atomic<bool> *simulationRunning, *runnerFinished, *runnerPreempted, *preemptionRequest;
    
    /* Timing */
    Timer* simulationTimer;
    size_t* SPEEDUP;

    /* Runner specific*/
    Process* runningProcess = nullptr;
    
    public:
    Runner(
        Simulator* _simulator,
        ProcessList* _processList,
        std::vector<std::tuple<size_t, size_t, size_t>>* _executionLog,
        
        std::mutex* _runnerMtx,
        std::mutex* _schedulerMtx,
        std::condition_variable* _schedulerCV,
        std::condition_variable* _runnerCV,

        std::atomic<size_t>* _delayedProcesses,
        std::atomic<bool>* _simulationRunning,
        std::atomic<bool>* _runnerFinished,
        std::atomic<bool>* _runnerPreempted, std::atomic<bool>* _preemptionRequest,

        Timer* _simulationTimer, size_t* _SPEEDUP
    ):
    simulator(_simulator),
    processList(_processList), executionLog(_executionLog),
    runnerMtx(_runnerMtx), schedulerMtx(_schedulerMtx), schedulerCV(_schedulerCV), runnerCV(_runnerCV),
    delayedProcesses(_delayedProcesses), simulationRunning(_simulationRunning),
    runnerFinished(_runnerFinished), runnerPreempted(_runnerPreempted),
    preemptionRequest(_preemptionRequest), simulationTimer(_simulationTimer),
    SPEEDUP(_SPEEDUP)
    {}
    
    virtual void run();
};

class Runner_RR : public Runner{
    size_t timeQuantum;
    
    public: 
    Runner_RR(
        Simulator* _simulator,
        ProcessList* _processList,
        std::vector<std::tuple<size_t, size_t, size_t>>* _executionLog,

        std::mutex* _runnerMtx,
        std::mutex* _schedulerMtx,
        std::condition_variable* _schedulerCV,
        std::condition_variable* _runnerCV,

        std::atomic<size_t>* _delayedProcesses,
        std::atomic<bool>* _simulationRunning,
        std::atomic<bool>* _runnerFinished,
        std::atomic<bool>* _runnerPreempted, std::atomic<bool>* _preemptionRequest,

        Timer* _simulationTimer,
        size_t _timeQuantum, size_t* _SPEEDUP
    ):
    Runner(
        _simulator, _processList, _executionLog,
        _runnerMtx, _schedulerMtx, _schedulerCV, _runnerCV,
        _delayedProcesses, _simulationRunning, _runnerFinished, _runnerPreempted,
        _preemptionRequest, _simulationTimer, _SPEEDUP
    ),
    timeQuantum(_timeQuantum)
    {}
    
    void run() override;
    void setTimeQuantum(size_t _timeQuantum);
};
