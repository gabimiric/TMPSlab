#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "../PC.h"
#include "../factory/Director.h"
#include "../factory/PCBuilder.h"
#include "../models/GPU.h"
using namespace std;


class EmployeeManager {
private:
    static EmployeeManager* instance;
    string name;
    int pcsBuilt;
    vector<string> buildLog;

    // Private constructor
    EmployeeManager(const string& n) : name(n), pcsBuilt(0) {
        cout << "[System] Employee Manager " << name << " has started their shift.\n";
    }

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
        cout << "\n========== EMPLOYEE INFO ==========" << endl;
        cout << "Manager: " << name << endl;
        cout << "===================================" << endl;
    }
    
    // Manager coordinates the PC build with Director and Builder
    PC* buildPC(Director& director, PCBuilder& builder, const string& buildType) {
        cout << "\n[" << name << "] Received order for: " << buildType << endl;
        cout << "[" << name << "] Coordinating with Director for construction..." << endl;
        
        // Delegate to director based on build type
        if (buildType == "Full") {
            director.constructFullPC(builder);
        } else if (buildType == "Basic") {
            director.constructBasicPC(builder);
        } else if (buildType == "Graphics") {
            director.constructGraphicsPC(builder);
        } else if (buildType == "Custom") {
            director.constructCustomPC(builder, true);
        } else {
            director.constructFullPC(builder); // default
        }
        
        PC* pc = builder.getPC();
        pcsBuilt++;
        buildLog.push_back(buildType);
        
        cout << "[" << name << "] Build #" << pcsBuilt << " completed!" << endl;
        return pc;
    }
    
    // Manager validates GPU prototypes before use
    void validateGPU(GPU* gpu) {
        cout << "[" << name << "] Validating GPU configuration..." << endl;
        cout << "  ";
        gpu->show();
        cout << "[" << name << "] GPU validated and ready for installation." << endl;
    }
    
    // Show build summary at end of shift
    void showBuildSummary() const {
        cout << "\n========== BUILD SUMMARY ==========" << endl;
        cout << "Manager: " << name << endl;
        cout << "Total PCs Built: " << pcsBuilt << endl;
        cout << "Build Types: ";
        for (size_t i = 0; i < buildLog.size(); i++) {
            cout << buildLog[i];
            if (i < buildLog.size() - 1) cout << ", ";
        }
        cout << "\n===================================" << endl;
    }

    string getName() const { return name; }
};

EmployeeManager* EmployeeManager::instance = nullptr;