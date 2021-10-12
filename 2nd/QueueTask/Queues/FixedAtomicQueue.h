#pragma once

#include <atomic>
#include <vector>

#include "Semaphores/Semaphore.h"

#include "Queue.h"

// this queue is cyclic as it's fixed size
// [4][5][7][.][.][.][0][1][2][3] - example of data, [.] is unused [n]
//
// MaxSize is 10
// [0] - tail, tail = 2
// [7] - head, head = 6

template <class T, size_t MaxSize>
class FixedAtomicQueue : public IQueue<T> {
protected:
    std::array<std::atomic<T>, MaxSize> dataArray;

    size_t head = 0;
    size_t tail = 0;

    Semaphore pushSemaphore;
    Semaphore popSemaphore;

    std::atomic_flag isPoppingRunning = ATOMIC_FLAG_INIT;
    std::atomic_flag isPushingRunning = ATOMIC_FLAG_INIT;

public:
    FixedAtomicQueue()
        : popSemaphore(Semaphore(0))
        , pushSemaphore(Semaphore(MaxSize))
    {}

    bool pop(T &val) {
        using namespace std::chrono;

        if (!popSemaphore.WaitForToDecrease(100ms)) {
            return false;
        }

        while (isPoppingRunning.test_and_set()); // critical area begins

        val = dataArray[tail].load(std::memory_order_acquire);
        tail = (tail + 1) % MaxSize;

        isPoppingRunning.clear(); // critical area ends

        pushSemaphore.Increase();
        return true;
    }

    void push(T val) {
        using namespace std::chrono;

        pushSemaphore.WaitToDecrease();

        while (isPushingRunning.test_and_set()); // critical area begins

        dataArray[head].store(val, std::memory_order_release);
        head = (head + 1) % MaxSize;

        isPushingRunning.clear(); // critical area ends

        popSemaphore.Increase();
    }
};
