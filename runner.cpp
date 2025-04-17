#include "runner.hpp"

inline size_t Runner::getCurrentTime(){
    return (size_t)std::chrono::round<std::chrono::seconds>(std::chrono::system_clock::now()-*startTime).count();
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
            
            if(slice_end != slice_start){
                executionLog->push_back(std::make_tuple(runningProcess->pid, slice_start, slice_end));
            }
            else{
                runningProcess->servedBefore = false;
            }

            if(interrupted){
                size_t duration_s = slice_end-slice_start;
                std::cout << "[" << getCurrentTime() << "|Runner]: Preemption request received!\n";
            
                // 1. Update remaining burst time
                if (duration_s >= runningProcess->remainingBurstTime){
                    runningProcess->remainingBurstTime = 0;
                }
                else{
                    runningProcess->remainingBurstTime -= duration_s;
                }

                // 3. Put process back at front of processList
                processList->pushFront(runningProcess);
            
                // 5. Set runner status to preempted
                {
                    std::lock_guard<std::mutex> schedLock(*schedulerMtx);
                    runnerPreempted->store(true);
                }

                //6. Notify the scheduler that the runner is gracefully Preempted
                schedulerCV->notify_one();

                //7. Wait for the scheduler to perofrm the scheduling
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
            std::mutex tmpMtx;
            std::unique_lock<std::mutex> runnerLock(tmpMtx);
            runnerCV->wait(runnerLock,
                [this](){
                    return !processList->empty();
                });
        }

        else{
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
            size_t slice_start = getCurrentTime();
            
            std::cout<<"["<<getCurrentTime()<<"|Runner]: Running process P"<<runningProcess->pid<<"\n";

            //if this is the first time the process is being served, set the first response time
            if(!runningProcess->servedBefore){
                runningProcess->firstResponseTime = getCurrentTime();
                runningProcess->servedBefore = true;
            }

            //3. Wait for the minimum of the process's remainin burst time and time quantum expiry
            bool finished = timeQuantum >= runningProcess->remainingBurstTime;
            std::this_thread::sleep_for(std::chrono::seconds(finished ? runningProcess->remainingBurstTime : timeQuantum));

            size_t slice_end = getCurrentTime();
            
            //4. Store time slice in execution log
            if(slice_end != slice_start){
                executionLog->push_back(std::make_tuple(runningProcess->pid, slice_start, slice_end));
            }
            else{
                runningProcess->servedBefore = false;
            }

            if(!finished){
                size_t duration_s = slice_end-slice_start;
            
                // 1. Update remaining burst time
                if (duration_s >= runningProcess->remainingBurstTime)
                    runningProcess->remainingBurstTime = 0;
                else
                    runningProcess->remainingBurstTime -= duration_s;
            
                // 2. Put process back at front of processList
                processList->pushBack(runningProcess);
            }
            
            else{
                //we know that the process finished its burst time.
                runningProcess ->finishTime = slice_end;
                runningProcess -> remainingBurstTime = 0;
                std::cout<<"["<<getCurrentTime()<<"|Runner]: Process P"<<runningProcess->pid<<" - Finished\n";
            }
        }
        
        else if((*delayedProcesses) != 0){
            std::cout<<"["<<getCurrentTime()<<"|Runner]: Waiting for new process...\n";
            runnerPreempted->store(true);
            std::mutex tmpMtx;
            std::unique_lock<std::mutex> runnerLock(tmpMtx);
            runnerCV->wait(runnerLock,
                [this](){
                    return !processList->empty();
                });
        }

        else{
            break;
        }
    }
    runnerFinished->store(true);
    std::cout<<"[Runner] No more processes to run.\n";
}

void Runner_RR::setTimeQuantum(size_t _timeQuantum){
    this->timeQuantum = _timeQuantum;
};