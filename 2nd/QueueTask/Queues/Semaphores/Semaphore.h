#pragma once

#include <atomic>
#include <chrono>
#include "PoorSemaphore.h"

class Semaphore {
protected:
    std::atomic_long count;
    PoorSemaphore sem;

public:
    explicit Semaphore(long initialCount);
    void Increase();
    void WaitToDecrease();
    bool WaitForToDecrease(std::chrono::milliseconds time);
};

