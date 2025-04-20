#include "simulator.hpp"

void Timer::incrementTime_ms(size_t increment){
    currentTime_ms += increment;
    timerCV.notify_all(); // Notify all waiting threads
    simulator->updateSimulationTimeGUI();
}

size_t Timer::getTime_ms(){
    std::lock_guard<std::mutex> lock(timerMutex);
    return currentTime_ms;
}

size_t Timer::getTime_s(){
    std::lock_guard<std::mutex> lock(timerMutex);
    return (currentTime_ms)/1000;
}

void Timer::waitForTime_ms(size_t targetTime_ms){
    std::mutex waitMutex;
    std::unique_lock<std::mutex> lock(waitMutex);
    timerCV.wait(lock, [this, targetTime_ms](){
        return currentTime_ms >= targetTime_ms;
    });
}

void Timer::stopTimer(){
    std::lock_guard<std::mutex> lock(timerMutex);
    timerRunning = false;
}

void Timer::resetTimer(){
    currentTime_ms = 0;
    simulator->updateSimulationTimeGUI();
}
