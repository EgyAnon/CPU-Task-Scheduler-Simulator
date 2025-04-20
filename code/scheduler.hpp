#pragma once
#include "process.hpp"
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <thread>
#include <chrono>
#include <iostream>
#include <queue>
#include "timer.hpp"

/**
 * Abstract base class for all scheduling algorithms.
 * Handles common structure, while leaving actual insertion logic to subclasses.
 */

class Scheduler {
    private:
    /* Data Structures */
    ProcessList* processList;
    std::vector<Process>* processVector;
    std::queue<Process*>* scheduleQueue;
    
    /*Mutexes*/
    std::mutex* processVectorMtx;
    std::mutex* scheduleQueueMtx;
    std::mutex* runnerMtx;
    std::mutex* schedulerMtx;
    
    /*Conditional Variables*/
    std::condition_variable* schedulerCV;
    std::condition_variable* runnerCV;
    std::condition_variable* queueCV;
    
    /* Signals */
    std::atomic<size_t>* delayedProcesses;
    std::atomic<bool>* runnerFinished, *runnerPreempted, *preemptionRequest;

    /*Timer*/
    Timer* simulationTimer;
    
    /* Scheduler-specific */
    std::function<bool(Process*, Process*)> compareRule;
    bool isPreemptive;
    
    public:
    Scheduler(
        /*Data Structures*/
        ProcessList* _processList,
        std::queue<Process*>* _scheduleQueue,

        /*Mutexes*/
        std::mutex* _scheduleQueueMtx,
        std::mutex* _runnerMtx,
        std::mutex* _schedulerMtx,

        /*Conditional Variables*/
        std::condition_variable* _schedulerCV,
        std::condition_variable* _runnerCV,
        std::condition_variable* _queueCV,

        /* Signals */
        std::atomic<size_t>* _delayedProcesses,
        std::atomic<bool>* _runnerFinished,
        std::atomic<bool>* _runnerPreempted,
        std::atomic<bool>* _preemptionRequest,

        /*Timer*/
        Timer* _simulationTimer,
        /* Scheduler-specific */
        std::function<bool(Process*, Process*)> _compareRule = Process::CompareByArrivalTime,
        bool _isPreemptive = false
    ):
    processList(_processList),
    scheduleQueue(_scheduleQueue),
    scheduleQueueMtx(_scheduleQueueMtx),
    runnerMtx(_runnerMtx),
    schedulerMtx(_schedulerMtx),
    schedulerCV(_schedulerCV),
    runnerCV(_runnerCV),
    queueCV(_queueCV),
    delayedProcesses(_delayedProcesses),
    runnerFinished(_runnerFinished),
    runnerPreempted(_runnerPreempted),
    preemptionRequest(_preemptionRequest),
    simulationTimer(_simulationTimer),
    compareRule(_compareRule),
    isPreemptive(_isPreemptive)
    {}

    void insertProcess(Process* newProcess);
    void setCompareRule(std::function<bool(Process*, Process*)> newCompareRule){
        compareRule = newCompareRule;
    }
    void setPreemptive(bool newIsPreemptive){
        isPreemptive = newIsPreemptive;
    }
    void run();
};
