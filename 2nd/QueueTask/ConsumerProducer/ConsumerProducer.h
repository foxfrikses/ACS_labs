#pragma once

#include "../Queues/Queue.h"

#include <inttypes.h>
#include <chrono>

class Consumer {
protected:
    IQueue<uint8_t>* const queue = nullptr;
public:
    explicit Consumer(IQueue<uint8_t>* q);

    struct RetType {uint32_t duration = 0; uint32_t result = 0;};

    RetType operator()();
};

class Producer {
protected:
    IQueue<uint8_t>* const queue = nullptr;
public:
    explicit Producer(IQueue<uint8_t>* q);

    struct RetType {uint32_t duration = 0;};

    RetType operator()(size_t taskCount);
};
