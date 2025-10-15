#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "models/GPU.h"
#include "models/RAM.hpp"
using namespace std;

class PC {
private:
    string type;
    string cpu;
    GPU *gpu;
    vector<RAM *> ramModules;

public:
    PC(const string &t) : type(t), gpu(nullptr) {}

    void setCPU(const string &c) { cpu = c; }
    void setGPU(GPU *g) { gpu = g; }
    void addRAM(RAM *r) { ramModules.push_back(r); }

    string getName() const { return type; }

    // Validation methods for POST
    bool hasCPU() const { return !cpu.empty(); }
    bool hasGPU() const { return gpu != nullptr; }
    bool hasRAM() const { return !ramModules.empty(); }
    int getRAMModuleCount() const { return ramModules.size(); }

    int getTotalRAMCapacity() const
    {
        int total = 0;
        for (const auto &r: ramModules)
        {
            total += r->getCapacity();
        }
        return total;
    }

    void showSpecs() const
    {
        cout << "\n--- " << type << " Specifications ---" << endl;
        cout << "CPU: " << (cpu.empty() ? "Not installed" : cpu) << endl;

        if (!ramModules.empty())
        {
            cout << "RAM: " << ramModules.size() << " modules, "
                    << getTotalRAMCapacity() << "GB total" << endl;
        }
        else
        {
            cout << "RAM: Not installed" << endl;
        }

        if (gpu)
            gpu->show();
        else
            cout << "GPU: Not installed" << endl;

        cout << "-------------------------------" << endl;
    }

    void showRAMInfo() const
    {
        if (!ramModules.empty())
        {
            cout << "\nDetailed RAM Configuration:" << endl;
            for (const auto &r: ramModules)
            {
                r->showInfo();
            }
        }
        else
        {
            cout << "\nNo RAM modules installed" << endl;
        }
    }

    ~PC()
    {
        delete gpu;
    }
};
