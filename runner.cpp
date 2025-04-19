#include "runner.hpp"
#include "simulator.hpp"


void Runner::run(){
    while(*delayedProcesses || !processList->Empty()){
        //1. Try to get the next process from the processList
        this->runningProcess = processList->GetHeadProcess();        

        //2. If there is a process, run it.
        if(runningProcess){
            runnerPreempted->store(false);
            //obtain the time slice start time
            size_t slice_start = simulationTimer->getTime_ms();
            
            //std::cout<<"["<<slice_start<<"|Runner]: Running process P"<<runningProcess->pid<<"\n";

            //if this is the first time the process is being served, set the first response time
            if(!runningProcess->servedBefore){
                runningProcess->first_response_time = slice_start;
                runningProcess->servedBefore = true;
            }

            //3. Wait for the process to finish or for a preemption request
            std::unique_lock<std::mutex> runnerLock(*runnerMtx);
            bool interrupted = false;
            while(!interrupted && runningProcess->remaining_burst_time>0){
                interrupted = runnerCV->wait_for(
                    runnerLock,
                    std::chrono::milliseconds(1000/(*SPEEDUP)),
                    [this](){
                        return preemptionRequest->load();
                    }
                );  
                if(!interrupted){
                    simulationTimer->incrementTime_ms(1000);
                    runningProcess->remaining_burst_time -= 1000;
                    simulator->updateSimulationTimeGUI();
                    simulator->onBurstTimeDecremented(runningProcess->pid, runningProcess->remaining_burst_time);
                }
            }

            size_t slice_end = simulationTimer->getTime_ms();
            
            if(slice_end != slice_start){
                executionLog->push_back(
                    std::make_tuple(
                        runningProcess->pid,
                        slice_start,
                        slice_end
                    )
                );
            }
        
            else{
                runningProcess->servedBefore = false;
            }

            if(interrupted){
                processList->PushFront(runningProcess);
            
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
                runningProcess ->finish_time = simulationTimer->getTime_ms();
                /* std::cout<<"["<<simulationTimer->getTime_s()<<"|Runner]: Process P"
                         <<runningProcess->pid<<" - Finished\n";  */
            }
            runnerLock.unlock(); //release the runner lock
        }
        
        else if((*delayedProcesses) != 0){
            /* std::cout   <<"["<<simulationTimer->getTime_s() */
            /*             <<"|Runner]: Waiting for new process...\n"; */ 
                        
            bool interrupted = false;
            std::mutex tmpMtx;
            std::unique_lock<std::mutex> runnerLock(*runnerMtx);
            runnerPreempted->store(true);
            schedulerCV->notify_one();

            while(!interrupted){
                interrupted = runnerCV->wait_for(
                    runnerLock,
                    std::chrono::milliseconds(1000/(*SPEEDUP)),
                    [this](){
                        return !processList->Empty();
                    }
                );
                if(!interrupted){
                    simulationTimer->incrementTime_ms(1000);
                    simulator->updateSimulationTimeGUI();
                }
                /* std::cout<<"Current Time: "<<simulationTimer->getTime_s()<<"\n"; */
            }
        }

        else{
            break;
        }
    }
    runnerFinished->store(true);
    simulator->simulationFinished();
    //td::cout<<"[Runner] No more processes to run.\n";
}

void Runner_RR::run(){
    while(*delayedProcesses || !processList->Empty()){
        //1. Try to get the next process from the processList
        this->runningProcess = processList->GetHeadProcess();        

        //2. If there is a process, run it.
        if(runningProcess){
            runnerPreempted->store(false);
            //obtain the time slice start time
            size_t slice_start = simulationTimer->getTime_ms();
            
            /* std::cout<<"["<<simulationTimer->getTime_ms()
            <<"|Runner]: Running process P" <<runningProcess->pid<<"\n"; */

            //if this is the first time the process is being served, set the first response time
            if(!runningProcess->servedBefore){
                runningProcess->first_response_time = simulationTimer->getTime_ms();
                runningProcess->servedBefore = true;
            }

            //3. Wait for the minimum of the process's remainin burst time and time quantum expiry
            bool finished = timeQuantum >= runningProcess->remaining_burst_time;
            size_t sleep_duration_ms =  ((finished)?runningProcess->remaining_burst_time:timeQuantum);
            while(sleep_duration_ms>0){
                std::this_thread::sleep_for(std::chrono::milliseconds(1000/(*SPEEDUP)));
                simulationTimer->incrementTime_ms(1000);
                sleep_duration_ms-=1000;
                runningProcess->remaining_burst_time -= 1000;
                simulator->updateSimulationTimeGUI();
                simulator->onBurstTimeDecremented(runningProcess->pid, runningProcess->remaining_burst_time);
            }

            size_t slice_end = simulationTimer->getTime_ms();
            
            //4. Store time slice in execution log
            if(slice_end != slice_start){
                executionLog->push_back(std::make_tuple(runningProcess->pid, slice_start, slice_end));
            }

            else{
                runningProcess->servedBefore = false;
            }

            if(!finished){ 
                // 2. Put process back at front of processList
                processList->PushBack(runningProcess);
            }
            
            else{
                //we know that the process finished its burst time.
                runningProcess ->finish_time = slice_end;
                /* std::cout<<"["<<simulationTimer->getTime_ms()<<"|Runner]: Process P"<<runningProcess->pid<<" - Finished\n"; */
            }
        }
        
        else if((*delayedProcesses) != 0){
            /* std::cout   <<"["<<simulationTimer->getTime_ms()
            <<"|Runner]: Waiting for new process...\n"; */
            std::mutex tmpMtx;
            std::unique_lock<std::mutex> runnerLock(tmpMtx);
            runnerPreempted->store(true);
            schedulerCV->notify_all();
            bool interrupted = false;
            while(!interrupted){
                interrupted = runnerCV->wait_for(
                    runnerLock,
                    std::chrono::milliseconds(500/(*SPEEDUP)),
                    [this](){
                        return !processList->Empty();
                    }
                );
                if(!interrupted){
                    simulationTimer->incrementTime_ms(500);
                    simulator->updateSimulationTimeGUI();
                }
            }
        }

        else{
            break;
        }
    }
    runnerFinished->store(true);
    simulator->simulationFinished();
    /* std::cout<<"[Runner] No more processes to run.\n"; */
}

void Runner_RR::setTimeQuantum(size_t _timeQuantum){
    this->timeQuantum = _timeQuantum;
};
