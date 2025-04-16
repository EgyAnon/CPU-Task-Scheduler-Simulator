#include "simulator.hpp"

int main(){
    Simulator simulator{};

    simulator.setScheduler("SJF Preemptive");

    simulator.addProcess(0, 6, 1);
    simulator.addProcess(2, 3, 0);
    simulator.addProcess(1, 2, 3);
    //simulator.addProcess(10, 10, 2);
    simulator.addProcess(12, 4, 4);
    
    simulator.start();

    //P0: 0-5
    //P1: 5-8
    //nothing: 8-10
    //P2: 10-20
    //P3: 20-24
}
