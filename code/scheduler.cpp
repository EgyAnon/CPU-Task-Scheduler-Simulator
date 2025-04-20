#include "scheduler.hpp"

void Scheduler::insertProcess(Process* newProcess){
    processList->AddProcess(newProcess,compareRule);
}

void Scheduler::run(){
    while(delayedProcesses->load() || !runnerFinished->load() || !scheduleQueue->empty()){
        std::unique_lock<std::mutex> queueLock(*scheduleQueueMtx);
        
        queueCV->wait(queueLock, [this](){
            return !scheduleQueue->empty() || (delayedProcesses->load() == 0);
        });

        while(!scheduleQueue->empty()){
            //get the process from the front of the queue

            Process* newProcess = scheduleQueue->front();

            scheduleQueue->pop();

            queueLock.unlock(); //release the queue lock

            if (this->isPreemptive) {
                // 1. Request preemption
                {
                    std::lock_guard<std::mutex> lock(*runnerMtx);
                    preemptionRequest->store(true);
                }
            
                // 2. Notify the runner
                runnerCV->notify_one();
            
                // 3. Wait for runner to confirm it has stopped
                {
                    std::unique_lock<std::mutex> schedLock(*schedulerMtx);
                    //std::cout<<"[Scheduler] waiting for runner to be preempted\n";
                    schedulerCV->wait(schedLock, [this]() {
                        return runnerPreempted->load();
                    });
                }
            }
            insertProcess(newProcess);
            (*delayedProcesses)--;
            queueLock.lock();
        }
        queueLock.unlock(); //release the queue lock   
        preemptionRequest->store(false); // Reset preemption request for next process
    
        runnerPreempted->store(false);  // Reset for next preemption
        runnerCV->notify_one(); // Notify the runner that a new process is available
    }
    std::cout<<"Scheduler Thread Finished!\n"; std::cout.flush();
}
