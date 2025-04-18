#include "process.hpp"

std::atomic<size_t> Process::_id{0};

Process::Process(size_t arrival_time, std::size_t burst_time, std::size_t priority):
    //all time is in milliseconds
    arrival_time(arrival_time),      
    burst_time(burst_time),          
    remaining_burst_time(burst_time),
    priority(priority),
    pid(_id)
{
    //ATOMIC INCREMENT
    _id++;   //To ensure all processes have a unique id.
}

void ProcessList::AddProcess(Process* new_process){
    ProcessNode* new_node = new ProcessNode(new_process);

    //Lock the list until the processes is added correctly 
    std::lock_guard<std::mutex> listLock(_list_mutex);
    if(!this->_head){
        _head = new_node;
        _tail = new_node;
        this->_size = 1;
    }
    else if(!_head || _size == 0){
        std::cout<<"INVALID STATE\n";
        return;
    }
    else{
        _tail->next = new_node;
        new_node->previous = _tail;
        _tail = new_node;
        this->_size++;
    }
}

void ProcessList::AddProcess(Process* new_process, std::function<bool(Process*, Process*)> CompareRule){
    ProcessNode* new_node = new ProcessNode(new_process);
    
    std::lock_guard<std::mutex> listLock(_list_mutex);
    if(!this->_head && _size == 0){
        _head = new_node;
        _tail = new_node;
        this->_size = 1;
    }
    else if(!_head || _size == 0){
        std::cout<<"INVALID STATE\n";
        return;
    }
    
    else{
        ProcessNode* current = _head;
        while(current && CompareRule(new_process,current->process)){
            current = current->next;
        }
        
        if(current == _head){
            new_node->next = _head;
            _head->previous = new_node;
            _head = new_node;
        }
        else if(current == nullptr){
            _tail->next = new_node;
            new_node->previous = _tail;
            _tail = new_node;
        }
        else{
            current->previous->next = new_node;
            new_node->previous = current->previous;
            current->previous = new_node;
            new_node->next = current;
        }
        this->_size++;
    }
}

bool Process::CompareByArrivalTime(Process* p1, Process* p2){
    if(p1->arrival_time == p2->arrival_time){
        return p1->pid > p2->pid; //if arrival time is the same, sort by pid
    }
    return p1->arrival_time> p2->arrival_time;
};
bool Process::CompareByRemainingBurstTime(Process* p1, Process* p2){
    if(p1->remaining_burst_time == p2->remaining_burst_time){
        return p1->pid > p2->pid; //if arrival time is the same, sort by pid
    }
    return p1->remaining_burst_time> p2->remaining_burst_time;
};
bool Process::CompareByPriority(Process* p1, Process* p2){
    if(p1->priority == p2->priority){
        return p1->pid > p2->pid; //if arrival time is the same, sort by pid
    }
    return p1->priority > p2->priority;
};
bool Process::CompareFalse(Process* p1, Process* p2){
    (void)p1; (void)p2;
    return false;
};

bool ProcessList::Empty(){
    std::lock_guard<std::mutex> listLock(_list_mutex);
    return _size == 0; 
}

Process* ProcessList::GetHeadProcess(){
    std::lock_guard<std::mutex> listLock(_list_mutex);
    if(!_head){
        return nullptr;
    }
    Process* returned_head;
    
    if(this->_size == 1){
        returned_head = _head->process;
        _head = nullptr;
        _tail = nullptr;
    }
    
    else{
        returned_head = _head->process;
        _head = _head->next;
        _head->previous = nullptr;
    }
    this->_size--;
    return returned_head;
}

void ProcessList::PushFront(Process* p) {
    ProcessNode* new_node = new ProcessNode(p);
    std::lock_guard<std::mutex> lock(_list_mutex);

    if (!_head) {
        _head = _tail = new_node;
    } else {
        new_node->next = _head;
        _head->previous = new_node;
        _head = new_node;
    }
    _size++;
    //std::cout<<"after push front:\n"; printList();
}

void ProcessList::PushBack(Process* p) {
    ProcessNode* new_node = new ProcessNode(p);
    std::lock_guard<std::mutex> lock(_list_mutex);
    if (!_head) {
        _head = _tail = new_node;
    } else {
        _tail->next = new_node;
        new_node->previous = _tail;
        _tail = new_node;
    }
    _size++;
}

void ProcessList::PrintList(){
    ProcessNode* current = _head;
    while(current){
        std::cout<<"P"<<current->process->pid<<" -> ";
        current = current->next;
    }
    std::cout<<"\nList Size: "<<this->_size<<"\n";
}

void Process::resetID(){
    Process::_id = 0;
}