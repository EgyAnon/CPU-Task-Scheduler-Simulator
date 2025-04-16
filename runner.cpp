#include "runner.hpp"

inline size_t Runner::getCurrentTime(){
    std::chrono::system_clock::time_point currentTimePoint = std::chrono::system_clock::now();
    size_t currentTime = (size_t)std::chrono::round<std::chrono::seconds>(currentTimePoint-*startTime).count();
    return currentTime;
}

void Runner::run(){
    while(*delayedProcesses || !processList->empty()){
        //1. Try to get the next process from the processList
        this->runningProcess = processList->getHeadProcess();        

        //2. If there is a process, run it.
        if(runningProcess){
            runnerPreempted->store(false);
            //obtain the time slice start time
            size_t slice_start = getCurrentTime();
            //auto slice_start = std::chrono::system_clock::now();
            
            std::cout<<"["<<slice_start<<"|Runner]: Running process P"<<runningProcess->pid<<"\n";

            //if this is the first time the process is being served, set the first response time
            if(!runningProcess->servedBefore){
                runningProcess->firstResponseTime = slice_start;
                runningProcess->servedBefore = true;
            }

            //3. Wait for the process to finish or for a preemption request
            std::unique_lock<std::mutex> runnerLock(*runnerMtx);
            bool interrupted = runnerCV->wait_for(runnerLock, std::chrono::seconds(runningProcess->remainingBurstTime),
            [this](){
                return preemptionRequest->load();
            });
            
            size_t slice_end = getCurrentTime();
            
            if(interrupted){
                size_t duration_s = slice_end-slice_start;
                std::cout << "[" << getCurrentTime() << "|Runner]: Preemption request received!\n";
            
                // 1. Update remaining burst time
                if (duration_s >= runningProcess->remainingBurstTime)
                    runningProcess->remainingBurstTime = 0;
                else
                    runningProcess->remainingBurstTime -= duration_s;
                /* executionLog->push_back(
                    std::make_tuple(
                        runningProcess->pid,
                        slice_start,
                        slice_end
                    ));
             */
                /* std::cout << "[" << getCurrentTime() << "|Runner]: Spent " << duration_s
                            << " seconds in process P" << runningProcess->pid << ". "
                            << runningProcess->remainingBurstTime << " seconds remaining\n";
                */
                // 2. Put process back at front of processList
                processList->pushFront(runningProcess);
                runningProcess = nullptr;
            
                // 4. Notify scheduler that runner has completed preemption
                {
                    std::lock_guard<std::mutex> schedLock(*schedulerMtx);
                    runnerPreempted->store(true);
                }

                schedulerCV->notify_one();

                {
                    runnerCV->wait(runnerLock, [this](){
                        return !runnerPreempted->load();
                    });
                }
                
            }
            
            else{
                //we know that the process finished its burst time.
                runningProcess ->finishTime = getCurrentTime();
                runningProcess -> remainingBurstTime = 0;
                //TODO: executionLog 
                std::cout<<"["<<getCurrentTime()<<"|Runner]: Process P"<<runningProcess->pid<<" - Finished\n";
            }
            runnerLock.unlock(); //release the runner lock
        }
        
        else if((*delayedProcesses) != 0){
            std::cout<<"["<<getCurrentTime()<<"|Runner]: Waiting for new process...\n";
            runnerPreempted->store(true);
            std::unique_lock<std::mutex> runnerLock(*runnerMtx);
            runnerCV->wait(runnerLock,
                [this](){
                    return !processList->empty();
                });
        }

        else{
            runnerFinished->store(true);
            break;
        }
    }
    runnerFinished->store(true);
    std::cout<<"[Runner] No more processes to run.\n";
}

void Runner_RR::run(){
    while(*delayedProcesses || !processList->empty()){
        //1. Try to get the next process from the processList
        this->runningProcess = processList->getHeadProcess();        

        //2. If there is a process, run it.
        if(runningProcess){
            runnerPreempted->store(false);
            //obtain the time slice start time
            auto slice_start = std::chrono::system_clock::now();
            
            std::cout<<"["<<getCurrentTime()<<"|Runner]: Running process P"<<runningProcess->pid<<"\n";

            //if this is the first time the process is being served, set the first response time
            if(!runningProcess->servedBefore){
                runningProcess->firstResponseTime = getCurrentTime();
                runningProcess->servedBefore = true;
            }

            //3. Wait for the minimum of the process's remainin burst time and time quantum expiry
            bool finished = timeQuantum >= runningProcess->remainingBurstTime;
            std::this_thread::sleep_for(std::chrono::seconds(finished ? runningProcess->remainingBurstTime : timeQuantum));

            auto slice_end = std::chrono::system_clock::now();
            
            if(!finished){
                auto duration_s = static_cast<unsigned long long>(std::chrono::round<std::chrono::seconds>(slice_end - slice_start).count());
                //std::cout << "[" << getCurrentTime() << "|Runner]: Preemption request received!\n";
            
                // 1. Update remaining burst time
                if (duration_s >= runningProcess->remainingBurstTime)
                    runningProcess->remainingBurstTime = 0;
                else
                    runningProcess->remainingBurstTime -= duration_s;
            
                // 2. Put process back at front of processList
                processList->pushBack(runningProcess);
                runningProcess = nullptr;  
            }
            
            else{
                //we know that the process finished its burst time.
                runningProcess ->finishTime = (size_t)std::chrono::round<std::chrono::seconds>(slice_end-*startTime).count();
                runningProcess -> remainingBurstTime = 0;
                std::cout<<"["<<getCurrentTime()<<"|Runner]: Process P"<<runningProcess->pid<<" - Finished\n";
            }
        }
        
        else if((*delayedProcesses) != 0){
            std::cout<<"["<<getCurrentTime()<<"|Runner]: Waiting for new process...\n";
            runnerPreempted->store(true);
            std::unique_lock<std::mutex> runnerLock(*runnerMtx);
            runnerCV->wait(runnerLock,
                [this](){
                    return !processList->empty();
                });
        }

        else{
            runnerPreempted->store(true);
            break;
        }
    }
    runnerFinished->store(true);
    std::cout<<"[Runner] No more processes to run.\n";
}