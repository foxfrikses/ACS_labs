#include "ConsumerProducer.h"

Consumer::Consumer(IQueue<uint8_t>* q)
    : queue(q)
{}

Producer::Producer(IQueue<uint8_t>* q)
        : queue(q)
{}

Consumer::RetType Consumer::operator()() {
    if (!queue) return {};

    using namespace std::chrono;

    RetType result;

    auto start{high_resolution_clock::now()};

    for (uint8_t i; queue->pop(i); result.result += i);

    auto end{high_resolution_clock::now()};
    result.duration = duration_cast<milliseconds>(end - start).count();

    return result;
}

Producer::RetType Producer::operator()(size_t taskCount) {
    if (!queue) return {};

    using namespace std::chrono;

    RetType result;

    auto start{high_resolution_clock::now()};

    for (size_t i = 0u; i < taskCount; ++i) {
        queue->push(1u);
    }

    auto end{high_resolution_clock::now()};
    result.duration = duration_cast<milliseconds>(end - start).count();

    return result;
}
