#include "Tasks/task.h"
#include "Tasks/task_1.h"
#include "Tasks/task_2.h"

#include <iostream>


int main() {
    while (true) {
        std::system("clear");
        std::cout << "Select task:" << "\n\n";
        std::cout << "1. Array incrementing" << '\n';
        std::cout << "2. Dynamic queue" << '\n';
        std::cout << "3. Fixed mutex queue" << '\n';
        std::cout << "4. Fixed atomic queue" << '\n';
        std::cout << "0. Exit" << '\n';
        std::cout << "\n>> ";

        std::string input;
        std::cin >> input;

        int result;
        try {result = stoi(input);} catch (...) {continue;}
        if (result < 0 || result > 4) {continue;}
        if (result == 0) {
            std::system("clear");
            return 0;
        }


        std::unique_ptr<ITask> task;

        switch (result) {
            case 1: task = std::make_unique<Task_1>(); break;
            case 2: task = std::make_unique<Task_2_1>(); break;
            case 3: task = std::make_unique<Task_2_2>(); break;
            case 4: task = std::make_unique<Task_2_3>(); break;
            default: continue;
        }

        bool doCalculate = true;

        if (task->IsLastResultExist()) {
            int result;
            while (true) {
                std::system("clear");
                std::cout << "Select:" << "\n\n";
                std::cout << "1. Show last result" << '\n';
                std::cout << "2. Recalculate (take a long time)" << '\n';
                std::cout << "0. Back" << '\n';
                std::cout << "\n>> ";

                std::string input;
                std::cin >> input;

                try {result = stoi(input);} catch (...) {continue;}
                if (result < 0 || result > 2) {continue;}
                break;
            }

            switch (result) {
                case 1: doCalculate = false; break;
                case 2: doCalculate = true; break;
                case 0: continue;
            }
        }

        if (doCalculate) {
            std::system("clear");
            std::cout << "Calculating... (It may take more than 10 minutes)" << std::endl;
            task->CalculateAndStoreResult();
        }

        std::system("clear");
        std::cout << task->GetLastResult();

        int c;
        while((c = getchar()) != '\n' && c != EOF);
        std::cin.get();
    }
}
