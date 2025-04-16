#include "scheduler.hpp"

void Scheduler::insertProcess(Process* newProcess){
    processList->addProcess(newProcess,compareRule);
}

inline size_t Scheduler::getCurrentTime(){
    std::chrono::system_clock::time_point currentTimePoint = std::chrono::system_clock::now();
    size_t currentTime = (size_t)std::chrono::round<std::chrono::seconds>(currentTimePoint-*startTime).count();
    return currentTime;
}
void Scheduler::run(){
    while(*delayedProcesses || !runnerFinished->load() || !scheduleQueue->empty()){
      /*   std::cout<<"["<<getCurrentTime()<<"|Scheduler]: Waiting for processes to arrive.\n";
        std::cout<<"["<<getCurrentTime()<<"|Scheduler]: Delayed processes: "<<*delayedProcesses<<"\n";
        std::cout<<"["<<getCurrentTime()<<"|Scheduler]: Processes in queue: "<<scheduleQueue->size()<<"\n";
         */
        std::unique_lock<std::mutex> queueLock(*scheduleQueueMtx);
        //wait until there is a process in the queue
        queueCV->wait(queueLock, [this](){
            return !scheduleQueue->empty() || (*delayedProcesses) == 0;
        });

        while(!scheduleQueue->empty()){
            //get the process from the front of the queue
            Process* newProcess = scheduleQueue->front();
            scheduleQueue->pop();
            queueLock.unlock(); //release the queue lock
            
            if (this->isPreemptive) {
                /* std::cout << "[" << getCurrentTime() << "|Scheduler]: Preempting the runner for process P"
                          << newProcess->pid << "\n";
             */
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
                
                //std::cout << "[" << getCurrentTime() << "|Scheduler]: Runner responded and preempted.\n";
            }
            
            //std::cout<<"["<<getCurrentTime()<<"|Scheduler]: Scheduling process P"<<newProcess->pid<<"\n";
            insertProcess(newProcess);
            (*delayedProcesses)--;
            queueLock.lock();
        }
        queueLock.unlock(); //release the queue lock   
        preemptionRequest->store(false); // Reset preemption request for next process
    
        runnerPreempted->store(false);  // Reset for next preemption
        runnerCV->notify_one(); // Notify the runner that a new process is available
    }
}