cmake_minimum_required(VERSION 3.20)
project(2nd)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pthread")

add_executable(2nd main.cpp Tasks/task.h Tasks/task.cpp Tasks/task_2.h Tasks/task_1.h Tasks/task_1.cpp QueueTask/ConsumerProducer/ConsumerProducer.cpp QueueTask/ConsumerProducer/ConsumerProducer.h QueueTask/QueueTask.h QueueTask/Queues/Queue.h QueueTask/Queues/FixedAtomicQueue.h QueueTask/Queues/FixedMutexQueue.h QueueTask/Queues/DynamicQueue.h QueueTask/Queues/Semaphores/Semaphore.h QueueTask/Queues/Semaphores/Semaphore.cpp QueueTask/Queues/Semaphores/PoorSemaphore.h QueueTask/Queues/Semaphores/PoorSemaphore.cpp TextTable/TextTable.h ArrayTask/ArrayTask.h ArrayTask/ArrayTask.cpp ArrayTask/Counters/MutexCounter.h ArrayTask/Counters/AtomicCounter.h)
