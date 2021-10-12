#include "task.h"

#include <filesystem>
#include <fstream>
#include <iostream>

using namespace std;

string ITask::CalculateAndStoreResult() {
    string str = Calculate();

    ofstream fileOut;
    fileOut.open(GetResultStorageName());
    if (fileOut.is_open()) {
        fileOut << str;
        fileOut.close();
    }

    return str;
}

string ITask::GetLastResult() {
    ifstream fileIn;
    fileIn.open(GetResultStorageName());
    if (fileIn.is_open()) {
        stringstream ss;
        ss << fileIn.rdbuf();
        fileIn.close();
        return ss.str();
    }

    return "";
    //system("clear");
    //int c;
    //while((c = getchar()) != '\n' && c != EOF);
    //std::cin.get();
}
bool ITask::IsLastResultExist() {
    return filesystem::exists(GetResultStorageName());
}

