#pragma once

#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

class Simulator;

class Timer{
    private:
        bool timerRunning{true};
        size_t currentTime_ms{0};
        std::mutex timerMutex;
        Simulator* simulator;
        
        public:
        Timer(Simulator* _simulator):simulator(_simulator){}

        std::condition_variable timerCV;
        void incrementTime_ms(size_t increment);
        size_t getTime_ms();
        size_t getTime_s();
        void waitForTime_ms(size_t targetTime_ms);
        void stopTimer();
        void resetTimer();
};
