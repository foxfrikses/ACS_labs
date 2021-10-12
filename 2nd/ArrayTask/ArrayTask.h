#pragma once

#include <inttypes.h>
#include <atomic>
#include <mutex>
#include <vector>
#include <future>
#include <thread>

class IArrayTask {
public:
    void Run();
    void SetTaskCount(uint32_t taskCount);
    void SetThreadCount(uint32_t threadCount);
    auto GetDuration() const -> uint64_t;
    bool VerifyDataArray() const;

protected:
    uint64_t duration = 0;
    uint32_t threadCount = 1;

    std::vector<int> dataArray = {1};

    virtual void Calculate() = 0;
};

template <class CounterType, bool WithSleep>
class ArrayTask : public IArrayTask
{
protected:
    CounterType counter;

    void Calculate() override {
        using namespace std::chrono;

        counter = 0;

        std::vector<std::future<void>> futures(threadCount);

        auto start{high_resolution_clock::now()};
        for (auto &f: futures) f = std::async(&ArrayTask::IncrementNext, this);
        for (auto &f: futures) f.wait();
        auto end{high_resolution_clock::now()};

        IArrayTask::duration = duration_cast<milliseconds>(end - start).count();
    }

    void IncrementNext() {
        using namespace std::chrono;
        using namespace std::this_thread;

        uint32_t index;

        while(true) {
            index = counter++;

            if (index >= dataArray.size()) break;

            ++dataArray[index];

            if constexpr (WithSleep) sleep_for(10ns);
        }
    }
};