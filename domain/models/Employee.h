#pragma once
#include <iostream>
#include <string>
using namespace std;

class EmployeeManager {
private:
    static EmployeeManager* instance;
    string name;

    // Private constructor
    EmployeeManager(const string& n) : name(n) {}

public:
    // Delete copy and assignment constructors
    EmployeeManager(const EmployeeManager&) = delete;
    EmployeeManager& operator=(const EmployeeManager&) = delete;

    static EmployeeManager* getInstance(const string& n = "Default Manager") {
        if (instance == nullptr) {
            instance = new EmployeeManager(n);
        }
        return instance;
    }

    void showInfo() const {
        cout << "Employee Manager: " << name << endl;
    }
};

EmployeeManager* EmployeeManager::instance = nullptr;