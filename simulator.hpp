#pragma once
#include "scheduler.hpp"
#include "runner.hpp"
#include <QObject>

#define MAX_SPEEDUP 80

struct CompareByArrival {
    bool operator()(Process* a, Process* b) const;
};


class Simulator : public QObject
{
    Q_OBJECT

    signals:
    void signal_processBurstTimeUpdated(size_t pid, double newTime);
    void signal_simulationFinished(std::tuple<double,double,double> result);
    void signal_updateSimulationTime(double);

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
    Timer simulationTimer{this};
    
    Simulator(){processVector.reserve(100);}
    ~Simulator(){
        delete scheduler;
        delete runner;
    };

    size_t addProcess(size_t arrivalTime, size_t burstTime, size_t priority = 0);
    size_t addProcessDynamically(size_t burstTime, size_t priority = 0);
    size_t getSimulationTime();
    std::tuple<double,double,double> getStatistics();
    void setScheduler(std::string schedulerName, size_t timeQuantum = 1);
    void setSpeedup(size_t SPEEDUP = 1);
    bool IsRunning();
    void onBurstTimeDecremented(size_t pid, size_t newTime);
    void simulationFinished();
    void updateSimulationTimeGUI();
    void resetSimulationData();

    void start();
    
    void printExecutionLog();
};
    
