#include "simulator.hpp"
#include <stdexcept>
void Simulator::addProcess(Process P){
    Process* newProcess;
    {
        std::lock_guard<std::mutex> vectorLock(processVectorMtx);
        processVector[P.pid] = P;
        newProcess = &processVector[P.pid];
    }
    std::this_thread::sleep_for(std::chrono::seconds(P.arrivalTime));
    scheduler->insertProcess(newProcess);
}

void Simulator::addProcess(size_t arrivalTime, size_t burstTime, size_t priority){
    Process* newProcess;
    {
        std::lock_guard<std::mutex> vectorLock(processVectorMtx);
        Process P(arrivalTime, burstTime, priority);
        processVector[P.pid] = P;
        newProcess = &processVector[P.pid];
    }
    delayedProcesses++;
    std::thread delayedThread(&Simulator::delayedAdd, this, newProcess, arrivalTime);
    delayedThread.detach(); // Detach the thread to allow it to run independently
}

void Simulator::delayedAdd(Process* newProcess, size_t delay_s){
    std::mutex sleepingThreadsMtx;
    std::unique_lock<std::mutex> sleeping(sleepingThreadsMtx);
    delayedAddCV.wait(sleeping, [this](){
        return simulationRunning.load();
    });

    std::this_thread::sleep_for(std::chrono::seconds(delay_s));
    {
        std::lock_guard<std::mutex> queueLock(scheduleQueueMtx);
        scheduleQueue.push(newProcess);
    }
    queueCV.notify_one(); // Notify the scheduler that a new process is available
}
void Simulator::setScheduler(std::string schedulerName, size_t timeQuantum){
    if(simulationRunning){
        std::cout<<"Can't change scheduler type mid-simulation";
        return;
    }
    else{
        this->scheduler = new Scheduler(
            &processList, &processVector, &scheduleQueue,
            &processVectorMtx, &scheduleQueueMtx, &runnerMtx, &schedulerMtx,
            &schedulerCV, &runnerCV, &queueCV,
            &delayedProcesses,
            &runnerFinished, &runnerPreempted, &preemptionRequest,
            &startTime
        );
        if(schedulerName == "RR"){
            this->scheduler->compareRule = Process::compareFalse;
            this->scheduler->isPreemptive = false;
            setRunner("RR", timeQuantum);
        }

        else{
            if(schedulerName == "FCFS"){
                this->scheduler->compareRule = Process::compareByArrivalTime;
                this->scheduler->isPreemptive = false;
            }
            else if(schedulerName == "SJF Preemptive"){
                this->scheduler->compareRule = Process::compareByRemainingBurstTime;
                this->scheduler->isPreemptive = true;
            }
            else if(schedulerName == "SJF Non-Preemptive"){
                this->scheduler->compareRule = Process::compareByRemainingBurstTime;
                this->scheduler->isPreemptive = false;
            }

            else if(schedulerName == "Priority Preemptive"){
                this->scheduler->compareRule = Process::compareByPriority;
                this->scheduler->isPreemptive = true;
            }

            else if(schedulerName == "Priority Non-Preemptive"){
                this->scheduler->compareRule = Process::compareByPriority;
                this->scheduler->isPreemptive = false;
            }

            else if(schedulerName == "RR"){
                this->scheduler->compareRule = Process::compareFalse;
                this->scheduler->isPreemptive = false;
            }

            else{
                throw std::invalid_argument("Scheduler type not recognized!");
                return;
            }
            setRunner("Regular");
        }

    }
}

void Simulator::setRunner(std::string runnerType, size_t timeQuantum){
    if(simulationRunning){
        std::cout<<"Can't change runner type mid-simulation";
        return;
    }

    if(runnerType == "Regular"){
        this->runner = new Runner(
            &processList, &executionLog,
            &runnerMtx, &schedulerMtx,
            &schedulerCV, &runnerCV, 
            &delayedProcesses,
            &runnerFinished, &runnerPreempted, &preemptionRequest,
            &startTime
        );
    }
    else if(runnerType == "RR"){
        this->runner = new Runner_RR(
            &processList, &executionLog,
            &runnerMtx, &schedulerMtx,
            &schedulerCV, &runnerCV, 
            &delayedProcesses,
            &runnerFinished, &runnerPreempted, &preemptionRequest,
            &startTime, timeQuantum
        );
    }
    else{
        std::cout<<"Runner Type Invalid!\n";
    }
}

void Simulator::start(){
    if(simulationRunning){
        std::cout<<"Simulation already running!";
        return;
    }
    else{
        std::cout<<"Simulation started!\n";
        std::thread schedulerThread(&Scheduler::run, scheduler);
        std::thread runnerThread(&Runner::run, runner);
        
        simulationRunning.store(true);
        startTime = std::chrono::system_clock::now();
        delayedAddCV.notify_all(); // Notify all delayed threads to start adding processes
        
        schedulerThread.join();
        runnerThread.join();
    }
    simulationRunning = false;
    std::cout<<"Simulation finished!"<<std::endl;
    displayStatistics();
}

void Simulator::displayStatistics(){
    size_t totalTurnaroundTime = 0;
    size_t totalWaitingTime = 0;
    size_t totalResponseTime = 0;
    size_t totalProcesses = Process::id;

    size_t tmp_turnaround, tmp_waiting, tmp_response;

    for(size_t i = 0; i<totalProcesses; i++){
        tmp_turnaround = processVector[i].finishTime - processVector[i].arrivalTime;
        tmp_waiting = tmp_turnaround - processVector[i].burstTime;
        tmp_response = processVector[i].firstResponseTime - processVector[i].arrivalTime;
        std::cout<<"-----------------------------P"<<i<<"-----------------------------\n";
        std::cout<<"Arrival time for process P"<<i<<": "<<processVector[i].arrivalTime<<"\n";
        std::cout<<"Burst time for process P"<<i<<": "<<processVector[i].burstTime<<"\n";
        std::cout<<"Time until first response for process P"<<i<<": "<<tmp_response<<"\n";
        std::cout<<"Turnaround time for process P"<<i<<": "<<tmp_turnaround<<"\n";
        std::cout<<"Waiting time for process P"<<i<<": "<<tmp_waiting<<"\n";

        totalTurnaroundTime += tmp_turnaround;
        totalWaitingTime += tmp_waiting;
        totalResponseTime += tmp_response;
    }

    std::cout<<"==========================Average===============================\n";
    std::cout<<"Average turnaround time: "<<static_cast<double>(totalTurnaroundTime)/static_cast<double>(totalProcesses)<<"\n";
    std::cout<<"Average waiting time: "<<static_cast<double>(totalWaitingTime)/static_cast<double>(totalProcesses)<<"\n";
    std::cout<<"Average response time: "<<static_cast<double>(totalResponseTime)/static_cast<double>(totalProcesses)<<"\n";
    std::cout<<"===============================================================\n";
}