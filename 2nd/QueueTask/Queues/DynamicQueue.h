#pragma once

#include <mutex>
#include <deque>
#include <thread>
#include <chrono>

#include "Queue.h"

template <class T>
class DynamicQueue
        : public IQueue<T>
        , protected std::deque<T>
{
    std::mutex m;
public:
    void push(T val) {
        std::lock_guard<std::mutex> lck(m);
        std::deque<T>::push_back(val);
    }

    bool pop(T& val) {
        using namespace std;
        using namespace std::chrono;

        std::unique_lock<std::mutex> lck(m);
        if (deque<T>::empty()) {
            lck.unlock();
            std::this_thread::sleep_for(10ms);
            lck.lock();
            if (deque<T>::empty()) {
                return false;
            }
        }
        val = deque<T>::front();
        deque<T>::pop_front();
        return true;
    }

};
