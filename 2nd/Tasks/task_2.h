#pragma once

#include "task.h"

#include <array>
#include <sstream>
#include <cassert>
#include <string>

#include "../QueueTask/QueueTask.h"
#include "../TextTable/TextTable.h"

template<class QDataType, QueueType QType, size_t QSize, size_t ProducerCountsCount, size_t ConsumerCountsCount>
std::array<std::array<std::string, ConsumerCountsCount>, ProducerCountsCount>
Calculate(size_t taskNum,
          std::array<size_t, ProducerCountsCount> producerCount,
          std::array<size_t, ConsumerCountsCount> consumerCount)
{
    std::array<std::array<std::string, ConsumerCountsCount>, ProducerCountsCount> resultMatrix;

    for (size_t i_prodNum = 0u; i_prodNum < ProducerCountsCount; ++i_prodNum) {
        for (size_t i_consNum = 0u; i_consNum < ConsumerCountsCount; ++i_consNum) {
            QueueTask<QDataType, QType, QSize> queTsk;

            queTsk.SetTaskCount(taskNum);
            queTsk.SetProducerCount(producerCount[i_prodNum]);
            queTsk.SetConsumerCount(consumerCount[i_consNum]);
            queTsk.Run();
            auto qResult = queTsk.GetDuration();

            resultMatrix[i_prodNum][i_consNum] =
                    (qResult.has_value()
                    ? std::to_string(qResult.value()) + " ms"
                    : "error");
        }
    }

    return resultMatrix;
}

template<QueueType QType>
class Task_2 : public ITask {
protected:
    inline static constexpr size_t TaskNum = 4 * 1024 * 1024;
    inline static constexpr std::array<size_t, 3> ProducerNum{ 1, 2, 4 };
    inline static constexpr std::array<size_t, 3> ConsumerNum{ 1, 2, 4 };
    inline static constexpr std::array<size_t, 3> QueueSize  { 1, 4, 16 };

    std::string Calculate() override {
        std::vector<std::array<std::array<std::string, ConsumerNum.size()>, ProducerNum.size()>> matrices;

        if constexpr (QType == QueueType::dynamic) {
            matrices.push_back(::Calculate<uint8_t, QType, 0>(TaskNum, ProducerNum, ConsumerNum));
        } else {
            assert(3 == QueueSize.size()); // add push_back for each item in QueueSize

            matrices.push_back(::Calculate<uint8_t, QType, QueueSize[0]>(TaskNum, ProducerNum, ConsumerNum));
            matrices.push_back(::Calculate<uint8_t, QType, QueueSize[1]>(TaskNum, ProducerNum, ConsumerNum));
            matrices.push_back(::Calculate<uint8_t, QType, QueueSize[2]>(TaskNum, ProducerNum, ConsumerNum));
        }

        std::stringstream ss;
        ss << '\t';
        if constexpr (QType == QueueType::dynamic)
            ss << "Dynamic queue";
        if constexpr (QType == QueueType::fixed_mutex)
            ss << "Fixed mutex queue";
        if constexpr (QType == QueueType::fixed_atomic)
            ss << "Fixed atomic queue";
        ss << "\n\n";

        std::vector<std::string> firstRow = {"producers\\consumers"}, firstCol = {"producers\\consumers"};
        for (auto num : ConsumerNum) firstRow.push_back(std::to_string(num));
        for (auto num : ProducerNum) firstCol.push_back(std::to_string(num));

        for (size_t i_matrix = 0; i_matrix < matrices.size(); ++i_matrix) {
            auto &matrix = matrices[i_matrix];
            if (matrix.size() != ProducerNum.size()) {
                throw std::runtime_error("Matrix size error 1");
            }

            if constexpr (QType != QueueType::dynamic) {
                ss << "\t" << "Queue size is " << QueueSize[i_matrix] << "\n";
            }

            TextTable table;
            table.addRow(firstRow);
            for (size_t row = 0; row < matrix.size(); ++row) {
                if (matrix[row].size() != ConsumerNum.size()) {
                    throw std::runtime_error("Matrix size error 2");
                }

                table.add(firstCol[row + 1]);

                for (auto& item : matrix[row]) {
                    table.add(item);
                }

                table.endOfRow();
            }

            ss << table << "\n\n";
        }

        return ss.str();
    }
};

class Task_2_1 : public Task_2<QueueType::dynamic> {
protected:
    std::string GetResultStorageName() const override {return "task_2_1.txt";}
};

class Task_2_2 : public Task_2<QueueType::fixed_mutex> {
protected:
    std::string GetResultStorageName() const override {return "task_2_2.txt";}
};

class Task_2_3 : public Task_2<QueueType::fixed_atomic> {
protected:
    std::string GetResultStorageName() const override {return "task_2_3.txt";}
};

