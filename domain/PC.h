#pragma once
#include <iostream>
#include <string>
#include "models/GPU.h"
using namespace std;

class PC {
private:
    string type;
    string cpu;
    string ram;
    GPU* gpu;

public:
    PC(const string& t) : type(t), gpu(nullptr) {}

    void setCPU(const string& c) { cpu = c; }
    void setRAM(const string& r) { ram = r; }
    void setGPU(GPU* g) { gpu = g; }

    void showSpecs() const {
        cout << "\n--- " << type << " Specifications ---" << endl;
        cout << "CPU: " << cpu << endl;
        cout << "RAM: " << ram << endl;
        if (gpu)
            gpu->show();
        cout << "-------------------------------" << endl;
    }

    ~PC() {
        delete gpu;
    }
};