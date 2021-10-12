#pragma once

#include <mutex>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <inttypes.h>

class PoorSemaphore {
protected:
    long count;
    std::condition_variable canDecrementCondVar;
    std::mutex decrementMutex;

public:
    void Increase();
    void WaitToDecrease();
    bool WaitForToDecrease(std::chrono::milliseconds milliseconds );
};
