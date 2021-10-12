#include "Semaphore.h"

using namespace std::chrono;

Semaphore::Semaphore(long initialCount)
    : count(initialCount)
{}

void Semaphore::WaitToDecrease() {
    long lastCount = count--;
    if (lastCount <= 0) {
        sem.WaitToDecrease();
    }
}

void Semaphore::Increase() {
    long lastCount = count++;
    if (lastCount < 0) {
        sem.Increase();
    }
}

bool Semaphore::WaitForToDecrease(milliseconds time) {
    long lastCount = count--;
    if (lastCount <= 0) {
        return sem.WaitForToDecrease(time);
    }
    return true;
}