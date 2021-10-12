#include "task_1.h"

#include <map>
#include <memory>
#include <sstream>
#include <string>

#include "../ArrayTask/ArrayTask.h"
#include "../ArrayTask/Counters/AtomicCounter.h"
#include "../ArrayTask/Counters/MutexCounter.h"

#include "../TextTable/TextTable.h"


using namespace std;

std::string Task_1::GetResultStorageName() const {
    return "task_1.txt";
}

std::string Task_1::Calculate() {
    std::stringstream ss;

    ss << "Array task (" << std::to_string(taskCount) << " tasks)";

    map<string, shared_ptr<IArrayTask>> tasks;
    tasks.emplace("Atomic",            make_unique<ArrayTask<AtomicCounter<uint32_t>, false>>());
    tasks.emplace("Mutex",             make_unique<ArrayTask<MutexCounter<uint32_t>,  false>>());
    tasks.emplace("Atomic with sleep", make_unique<ArrayTask<AtomicCounter<uint32_t>, true>>());
    tasks.emplace("Mutex with sleep",  make_unique<ArrayTask<MutexCounter<uint32_t>,  true>>());

    for (uint threadCount: threadCounts) {
        TextTable table;

        for (auto& [taskName, task] : tasks) {
            task->SetTaskCount(taskCount);
            task->SetThreadCount(threadCount);
            task->Run();
            const bool verifyResult = task->VerifyDataArray();

            table.add(taskName);
            table.add(verifyResult ? to_string(task->GetDuration()) + " ms" : string("error"));
            table.endOfRow();
        }

        ss << "\n\n\t" << std::to_string(threadCount) + " threads";
        ss << "\n" << table;
    }

    return ss.str();
}

