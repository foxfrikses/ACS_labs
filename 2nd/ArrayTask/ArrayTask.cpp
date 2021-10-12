#include "ArrayTask.h"

#include <thread>
#include <chrono>

void IArrayTask::Run() {
    for (auto &elem : dataArray) elem = 0;

    Calculate();
}

bool IArrayTask::VerifyDataArray() const {
    for (auto elem : dataArray)
        if (elem != 1)
            return false;

    return true;
}

uint64_t IArrayTask::GetDuration() const {
   return duration;
}

void IArrayTask::SetTaskCount(uint32_t taskCount) {
    dataArray.resize(taskCount > 0 ? taskCount : 1);
}

void IArrayTask::SetThreadCount(uint32_t threadCount) {
    this->threadCount = (threadCount) ? threadCount : 1;
}

