#pragma once

#include "task.h"
#include <array>

class Task_1 : public ITask {
protected:
    inline static constexpr uint taskCount = 1024*1024;
    inline static constexpr std::array<uint, 4> threadCounts = {4, 8, 16, 32};

protected:
    std::string Calculate() override;

    std::string GetResultStorageName() const override;
};
