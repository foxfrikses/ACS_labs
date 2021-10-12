#pragma once

#include "Queues/Queue.h"
#include "Queues/DynamicQueue.h"
#include "Queues/FixedMutexQueue.h"
#include "Queues/FixedAtomicQueue.h"
#include "ConsumerProducer/ConsumerProducer.h"

#include <inttypes.h>
#include <vector>
#include <memory>
#include <optional>
#include <future>

template <class QueueDataType>
class IQueueTask {
public:
    explicit IQueueTask(std::unique_ptr<IQueue<QueueDataType>> q) : queue(std::move(q)) {}
    void Run();

    void SetProducerCount(uint32_t c){producerCount = (c > 0 ? c : 1u);};
    void SetConsumerCount(uint32_t c){consumerCount = (c > 0 ? c : 1u);};
    void SetTaskCount    (uint32_t c){taskCount     = (c > 0 ? c : 1000u);};

    std::optional<uint32_t> GetDuration() const {return duration;}

protected:
    std::unique_ptr<IQueue<QueueDataType>> queue;

    uint32_t  producerCount = 1;
    uint32_t  consumerCount = 1;
    uint32_t  taskCount = 1000;

    std::optional<uint32_t> duration = std::nullopt;
};

enum class QueueType {dynamic, fixed_mutex, fixed_atomic};

template <class QueueDataType, QueueType QType, size_t QSize = 1>
class QueueTask : public IQueueTask<QueueDataType>  {
public:
    QueueTask() : IQueueTask<QueueDataType>([]{
        std::unique_ptr<IQueue<QueueDataType>> q;
        if constexpr (QType == QueueType::fixed_mutex)
            q = std::make_unique<FixedMutexQueue<QueueDataType, QSize>>();
        if constexpr (QType == QueueType::fixed_atomic)
            q = std::make_unique<FixedAtomicQueue<QueueDataType, QSize>>();
        if constexpr (QType == QueueType::dynamic)
            q = std::make_unique<DynamicQueue<QueueDataType>>();
        return q;
    }())
    {}
};


template <class QueueDataType>
void IQueueTask<QueueDataType>::Run() {
    using namespace std;

    vector<future<Consumer::RetType>> consumerResults;
    vector<future<Producer::RetType>> producerResults;

    for (size_t i = 0; i < producerCount; ++i) {
        producerResults.push_back(async(Producer(queue.get()), taskCount));
    }
    for (size_t i = 0; i < consumerCount; ++i) {
        consumerResults.push_back(async(Consumer(queue.get())));
    }

    for (auto& f : consumerResults) f.wait();
    for (auto& f : producerResults) f.wait();

    uint32_t localResult = 0;
    uint32_t localDuration = 0;
    for (auto& result : consumerResults) {
        auto res = result.get();
        localResult += res.result;
        localDuration += res.duration;
    }
    for (auto& result : producerResults) {
        localDuration += result.get().duration;
    }

    if (localResult == taskCount * producerCount)
        duration = localDuration;
    else
        duration = nullopt;
}
