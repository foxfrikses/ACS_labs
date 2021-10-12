#pragma once

#include <string>
#include <ostream>

class ITask {
public:
    std::string CalculateAndStoreResult();
    std::string GetLastResult();
    bool IsLastResultExist();

protected:
    virtual std::string Calculate() = 0;
    virtual std::string GetResultStorageName() const = 0;
};
