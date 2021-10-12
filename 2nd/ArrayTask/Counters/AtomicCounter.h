#pragma once

template <typename IntType>
class AtomicCounter {
protected:
    std::atomic<IntType> counter = 0;
public:
    IntType operator=(IntType n) {
        return counter = n;
    }
    IntType operator++(int) {
        return counter++;
    }
};
