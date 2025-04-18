#include "simulator.hpp"
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

bool compareLogs(
    const std::vector<std::tuple<size_t, size_t, size_t>>& a,
    const std::vector<std::tuple<size_t, size_t, size_t>>& b
) {
    return a == b;
}

void setupSimulator(Simulator& simulator) {
    simulator.setScheduler("SJF Preemptive");
    simulator.setSpeedup(10);

    // Test 1–6: Base cases
    simulator.addProcess(0, 10); // P0
    simulator.addProcess(5, 3); // P1
    simulator.addProcess(15, 2); // P2
    simulator.addProcess(15, 2); // P3
    simulator.addProcess(20, 3); // P4
    simulator.addProcess(20, 2); // P5
    simulator.addProcess(26, 2); // P6
    simulator.addProcess(28, 3); // P7
    simulator.addProcess(28, 1); // P8
    simulator.addProcess(34, 2); // P9
    simulator.addProcess(35, 4); // P10

    // Test 7: Many processes at the same time
    for (int i = 0; i < 10; ++i) {
        simulator.addProcess(40, 1 + (i % 3)); // P11–P20
    }
    
    // Test 8: Back-to-back tiny jobs
    simulator.addProcess(70, 1); // P21
    simulator.addProcess(71, 1); // P22
    simulator.addProcess(72, 1); // P23
    
    // Test 9: One big process interrupted by short ones
    simulator.addProcess(75, 10); // P24
    for (int i = 0; i < 5; ++i) {
        simulator.addProcess(76 + i, 1); // P25–P29
    }

    // Test 10: End + arrival same time
    simulator.addProcess(95, 5); // P30
    simulator.addProcess(95, 2); // P31
}

int main() {
    const int NUM_RUNS = 20;
    std::vector<std::tuple<size_t, size_t, size_t>> baselineLog;
    bool allSame = true;

    for (int i = 0; i < NUM_RUNS; ++i) {
        Simulator sim = Simulator();
        setupSimulator(sim);
        sim.start();
        
        auto& log = sim.executionLog;

        if (i == 0) {
            baselineLog = log;
        } else if (!compareLogs(baselineLog, log)) {
            std::cout << "Mismatch found at run #" << i << "\n";
            allSame = false;
            break;
        }
        Process::resetID();
    }

    if (allSame) {
        std::cout << "All runs produced consistent execution logs.\n";
    } else {
        std::cout << "Inconsistent behavior detected.\n";
    }

    return 0;
}
