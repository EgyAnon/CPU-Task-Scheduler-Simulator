#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

class Timer{
    private:
        bool timerRunning{true};
        size_t currentTime_ms{0};
        std::mutex timerMutex;
        
        public:
        std::condition_variable timerCV;
        void incrementTime_ms(size_t increment){
            currentTime_ms += increment;
            timerCV.notify_all(); // Notify all waiting threads
        }

        size_t getTime_ms(){
            std::lock_guard<std::mutex> lock(timerMutex);
            return currentTime_ms;
        }

        size_t getTime_s(){
            std::lock_guard<std::mutex> lock(timerMutex);
            return (currentTime_ms)/1000;
        }

        void waitForTime_ms(size_t targetTime_ms){
            std::mutex waitMutex;
            std::unique_lock<std::mutex> lock(waitMutex);
            timerCV.wait(lock, [this, targetTime_ms](){
                return currentTime_ms >= targetTime_ms;
            });
        }

        void stopTimer(){
            std::lock_guard<std::mutex> lock(timerMutex);
            timerRunning = false;
        }

        void resetTimer(){
            std::lock_guard<std::mutex> lock(timerMutex);
            currentTime_ms = 0;
        }
};