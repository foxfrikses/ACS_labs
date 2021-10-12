#pragma once

#include <mutex>

template <typename IntType>
class MutexCounter {
protected:
    mutable std::mutex m;
    uint32_t counter = 0;
public:
    IntType operator=(IntType n) {
        std::lock_guard lck(m);
        return counter = n;
    }
    IntType operator++(int) {
        std::lock_guard lck(m);
        return counter++;
    }
};
