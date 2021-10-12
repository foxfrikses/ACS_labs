#include "PoorSemaphore.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

void PoorSemaphore::Increase() {
    std::unique_lock<std::mutex> lck(decrementMutex);
    ++count;
    canDecrementCondVar.notify_one();
}

void PoorSemaphore::WaitToDecrease() {
    std::unique_lock<std::mutex> lck(decrementMutex);
    while (count == 0) {
        canDecrementCondVar.wait(lck);
    }
    --count;
}

bool PoorSemaphore::WaitForToDecrease(milliseconds time) {
    std::unique_lock<std::mutex> lck(decrementMutex);

    auto untilTime = std::chrono::steady_clock::now() + time;

    while (count == 0) {
        if (canDecrementCondVar.wait_until(lck, untilTime) == std::cv_status::timeout) {
            return false;
        }
    }

    --count;
    return true;
}