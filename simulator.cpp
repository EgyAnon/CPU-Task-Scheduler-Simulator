#include "simulator.hpp"
#include <stdexcept>
#include <functional>

void Simulator::setSpeedup(size_t _speedup){
    if(_speedup == 0){
        throw std::invalid_argument("SPEEDUP MUST BE BETWEEN 1 AND 30. GIVEN VALUE: 0");
    }
    else if(_speedup> MAX_SPEEDUP){
        std::cout<<"MAX SPEEDUP IS "<<MAX_SPEEDUP<<"\n";
        Speedup = MAX_SPEEDUP;
    }
    else{
        Speedup = _speedup; 
    }
}


void Simulator::addProcess(size_t arrival_time, size_t burstTime, size_t priority){
    Process* newProcess;
    Process P(arrival_time*1000, burstTime*1000, priority);
    {
        std::lock_guard<std::mutex> vectorLock(processVectorMtx);
        processVector[P.pid] = P;
    }
    newProcess = &processVector[P.pid];
    delayedProcesses++;
    delayedQueue.push(newProcess);
}

void Simulator::addDelayedProcesses(){
    while(!delayedQueue.empty()){
        Process* top = delayedQueue.top();
        if(simulationTimer.getTime_ms() >= top->arrival_time){
            scheduleQueue.push(top);
            delayedQueue.pop();
            queueCV.notify_one();
        }
        else{
            std::mutex tmpMutex;
            std::unique_lock<std::mutex> lk(tmpMutex);
            simulationTimer.timerCV.wait(
                lk,
                [this, top](){
                    return (simulationTimer.getTime_ms()>= top->arrival_time);
                }
            );
        }
    }
}

void Simulator::addProcessDynamically(size_t _burstTime, size_t _priority){
    Process P(simulationTimer.getTime_ms(), _burstTime*1000, _priority);
    {
        std::lock_guard<std::mutex> lk(processVectorMtx);
        processVector[P.pid] = P;
    }
    Process* newProcess = &processVector[P.pid];
    delayedProcesses++;
    {
        std::lock_guard<std::mutex>  lk(scheduleQueueMtx);
        scheduleQueue.push(newProcess);
        std::cout<<"Added new process: P"<<P.pid<<"\n";
        queueCV.notify_one();
    }
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
            &runnerFinished, &runnerPreempted, &preemptionRequest

        );
        if(schedulerName == "RR"){
            this->scheduler->setCompareRule(Process::CompareFalse);
            this->scheduler->setPreemptive(false);
            setRunner("RR", timeQuantum*1000);
        }

        else{
            if(schedulerName == "FCFS"){
                this->scheduler->setCompareRule(Process::CompareByArrivalTime);
            this->scheduler->setPreemptive(false);
            }
            else if(schedulerName == "SJF Preemptive"){
                this->scheduler->setCompareRule(Process::CompareByRemainingBurstTime);
                this->scheduler->setPreemptive(true);
            }
            else if(schedulerName == "SJF Non-Preemptive"){
                this->scheduler->setCompareRule(Process::CompareByRemainingBurstTime);
                this->scheduler->setPreemptive(false);
            }

            else if(schedulerName == "Priority Preemptive"){
                this->scheduler->setCompareRule(Process::CompareByPriority);
                this->scheduler->setPreemptive(true);
            }

            else if(schedulerName == "Priority Non-Preemptive"){
                this->scheduler->setCompareRule(Process::CompareByPriority);
                this->scheduler->setPreemptive(false);
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
            &simulationTimer, &Speedup
        );
    }
    else if(runnerType == "RR"){
        this->runner = new Runner_RR(
            &processList, &executionLog,
            &runnerMtx, &schedulerMtx,
            &schedulerCV, &runnerCV, 
            &delayedProcesses,
            &runnerFinished, &runnerPreempted, &preemptionRequest,
            &simulationTimer, timeQuantum, &Speedup
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
        simulationRunning.store(true);

        std::thread delayedAdd(&Simulator::addDelayedProcesses, this);
        delayedAdd.detach();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::thread schedulerThread(&Scheduler::run, scheduler);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::thread runnerThread(&Runner::run, runner);
        
        schedulerThread.join();
        runnerThread.join();
    }
    simulationRunning = false;
    std::cout<<"Simulation finished!"<<std::endl;
    displayStatistics();
    displayExecutionLog();
}

void Simulator::displayStatistics(){
    size_t totalTurnaroundTime = 0;
    size_t totalWaitingTime = 0;
    size_t totalResponseTime = 0;
    size_t totalProcesses = Process::GetNumerOfProcesses();

    size_t tmp_turnaround, tmp_waiting, tmp_response;

    for(size_t i = 0; i<totalProcesses; i++){
        tmp_turnaround = processVector[i].finish_time - processVector[i].arrival_time;
        tmp_waiting = tmp_turnaround - processVector[i].burst_time;
        tmp_response = processVector[i].first_response_time- processVector[i].arrival_time;
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

void Simulator::displayExecutionLog(){
    std::cout<<"==========================Execution Log===============================\n";
    std::cout<<"Process ID\t|\tStart Time\t\t|\tEnd Time\n";
    for(auto& log : executionLog){
        std::cout<<"P"<<std::get<0>(log)<<"\t\t|\t\t"<<std::get<1>(log)/1000<<"\t\t|\t"<<std::get<2>(log)/1000<<"\n";
    }
    std::cout<<"=======================================================================\n";
}

bool CompareByArrival::operator()(Process* a, Process* b) const {
    return a->arrival_time > b->arrival_time; // Min-heap: earlier time = higher priority
}