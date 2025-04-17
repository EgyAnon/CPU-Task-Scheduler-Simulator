#include "process.hpp"

std::atomic<size_t> Process::id{0};

Process::Process(size_t _arrivalTime, std::size_t _burstTime, std::size_t _priority):
    //all time is in seconds
    arrivalTime(_arrivalTime),      
    burstTime(_burstTime),          
    remainingBurstTime(_burstTime),
    priority(_priority),
    pid(id)
{
    id++;
}

void ProcessList::addProcess(Process* newProcess){
    ProcessNode* newNode = new ProcessNode(newProcess);
    std::lock_guard<std::mutex> listLock(listMutex);
    if(!this->head){
        head = newNode;
        tail = newNode;
        this->size = 1;
    }
    else if(!head || size == 0){
        std::cout<<"INVALID STATE\n";
        return;
    }
    else{
        tail->next = newNode;
        newNode->previous = tail;
        tail = newNode;
        this->size++;
    }
    //std::cout<<"List after adding:\n"; printList();
}

void ProcessList::addProcess(Process* newProcess, std::function<bool(Process*, Process*)> compareRule){
    ProcessNode* newNode = new ProcessNode(newProcess);
    
    std::lock_guard<std::mutex> listLock(listMutex);
    //std::cout<<"List before adding:\n"; printList();
    if(!this->head && size == 0){
        head = newNode;
        tail = newNode;
        this->size = 1;
    }
    else if(!head || size == 0){
        std::cout<<"INVALID STATE\n";
        return;
    }
    
    else{
        ProcessNode* current = head;
        while(current && compareRule(newProcess,current->process)){
            current = current->next;
        }
        
        if(current == head){
            newNode->next = head;
            head->previous = newNode;
            head = newNode;
        }
        else if(current == nullptr){
            tail->next = newNode;
            newNode->previous = tail;
            tail = newNode;
        }
        else{
            current->previous->next = newNode;
            newNode->previous = current->previous;
            current->previous = newNode;
            newNode->next = current;
        }
        this->size++;
    }
    //std::cout<<"List after adding:\n"; printList();
}

bool Process::compareByArrivalTime(Process* p1, Process* p2){
    if(p1->arrivalTime == p2->arrivalTime){
        return p1->pid > p2->pid; //if arrival time is the same, sort by pid
    }
    return p1->arrivalTime > p2->arrivalTime;
};
bool Process::compareByRemainingBurstTime(Process* p1, Process* p2){
    if(p1->remainingBurstTime == p2->remainingBurstTime){
        return p1->pid > p2->pid; //if arrival time is the same, sort by pid
    }
    return p1->remainingBurstTime > p2->remainingBurstTime;
};
bool Process::compareByPriority(Process* p1, Process* p2){
    if(p1->priority == p2->priority){
        return p1->pid > p2->pid; //if arrival time is the same, sort by pid
    }
    return p1->priority > p2->priority;
};
bool Process::compareFalse(Process* P1, Process* P2){
    (void)P1; (void)P2;
    return false;
};

bool ProcessList::empty(){
    std::lock_guard<std::mutex> listLock(listMutex);
    return size == 0; 
}

Process* ProcessList::getHeadProcess(){
    std::lock_guard<std::mutex> listLock(listMutex);
    //std::cout<<"Popping head:\n"; printList();
    if(!head){
        //std::cout<<"List is empty! size = "<<size<<"\n";
        return nullptr;
    }
    Process* returnedHead;
    
    if(this->size == 1){
        returnedHead = head->process;
        head = nullptr;
        tail = nullptr;
    }
    
    else{
        returnedHead = head->process;
        head = head->next;
        head->previous = nullptr;
    }
    this->size--;
    //std::cout<<"After popping head:\n"; printList();
    return returnedHead;
}

void ProcessList::pushFront(Process* p) {
    ProcessNode* newNode = new ProcessNode(p);
    std::lock_guard<std::mutex> lock(listMutex);
    //std::cout<<"before push front:\n"; printList();
    if (!head) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head->previous = newNode;
        head = newNode;
    }
    size++;
    //std::cout<<"after push front:\n"; printList();
}

void ProcessList::pushBack(Process* p) {
    ProcessNode* newNode = new ProcessNode(p);
    std::lock_guard<std::mutex> lock(listMutex);
    if (!head) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->previous = tail;
        tail = newNode;
    }
    size++;
}

void ProcessList::printList(){
    ProcessNode* current = head;
    while(current){
        std::cout<<"P"<<current->process->pid<<" -> ";
        current = current->next;
    }
    std::cout<<"\nList Size: "<<this->size<<"\n";
}

void Process::resetID(){
    Process::id = 0;
}