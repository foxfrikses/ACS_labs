#pragma once

#include <inttypes.h>
#include <array>
#include <mutex>
#include <condition_variable>
#include <thread>

#include "Queue.h"

// this queue is cyclic as it's fixed size
// [4][5][7][.][.][.][0][1][2][3] - example of data, [.] is unused [n]
//
// MaxSize is 10
// [0] - tail, tail = 2
// [7] - head, head = 6

template <class T, size_t MaxSize>
class FixedMutexQueue : public IQueue<T>
{
protected:
    std::array<T, MaxSize> dataArray;
    std::mutex dataArrayMutex;

    size_t head = 0u;
    size_t tail = 0u;

    bool isEmpty = true;
    bool isFull  = false;

    std::condition_variable notFullCondVar;
    std::condition_variable notEmptyCondVar;

public:
    void push(T val) override {
        std::unique_lock<std::mutex> lck(dataArrayMutex);
        notFullCondVar.wait(lck, [this]{return !isFull;});

        dataArray[head] = val;
        head = (head + 1) % MaxSize;

        isFull  = (head == tail);
        isEmpty = false;

        notEmptyCondVar.notify_one();
    }

    bool pop(T& val) override {
        using namespace std::chrono;

        std::unique_lock<std::mutex> lck(dataArrayMutex);
        if (!notEmptyCondVar.wait_for(lck, 10ms, [this]{return !isEmpty;})) {
            return false;
        }

        val = dataArray[tail];
        tail = (tail + 1) % MaxSize;

        isFull  = false;
        isEmpty = (head == tail);

        notFullCondVar.notify_one();
        return true;
    }
};

