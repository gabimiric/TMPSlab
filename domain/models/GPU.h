#pragma once
#include <iostream>
#include <string>
using namespace std;

// Base GPU class with Prototype pattern
class GPU {
protected:
    string model;
    string cooler;
    int vramGB;
    int clockSpeedMHz; // in MHz
    int cores;
    int powerConsumption; // in Watts

public:
    GPU(const string &m, const string &c, int v, int clock, int cores, int power)
        : model(m), cooler(c), vramGB(v), clockSpeedMHz(clock), cores(cores), powerConsumption(power) {}

    virtual ~GPU() = default;

    // Pure virtual clone method - the key to Prototype pattern
    virtual GPU *clone() const = 0;

    virtual void show() const
    {
        cout << "GPU: " << model
             << " | Cooler: " << cooler
             << " | VRAM: " << vramGB << "GB"
             << " | Clock: " << clockSpeedMHz << " MHz"
             << " | Cores: " << cores << endl;
    }

    // Setters for customization after cloning
    void setCooler(const string &c) { cooler = c; }
    void setVRAM(int v) { vramGB = v; }
    void setClockSpeed(int clock) { clockSpeedMHz = clock; }
    void setCores(int c) { cores = c; }

    string getName() const
    {
        return model;
    }

    int getPowerConsumption() const {
        return powerConsumption;
    }
};

// Concrete GPU: RTX 3080
class RTX3080 : public GPU {
public:
    RTX3080(const string &cooler = "Stock", int vram = 10, int clock = 1710, int cores = 8704)
        : GPU("NVIDIA RTX 3080", cooler, vram, clock, cores, 320) {}

    RTX3080 *clone() const override
    {
        return new RTX3080(*this);
    }
};

// Concrete GPU: RX 6600
class RX6600 : public GPU {
public:
    RX6600(const string &cooler = "Stock", int vram = 8, int clock = 2491, int cores = 1792)
        : GPU("AMD Radeon RX 6600", cooler, vram, clock, cores, 132) {}

    RX6600 *clone() const override
    {
        return new RX6600(*this);
    }
};

// Concrete GPU: RTX 4090
class RTX4090 : public GPU {
public:
    RTX4090(const string &cooler = "Stock", int vram = 24, int clock = 2520, int cores = 16384)
        : GPU("NVIDIA RTX 4090", cooler, vram, clock, cores, 450) {}

    RTX4090 *clone() const override
    {
        return new RTX4090(*this);
    }
};

// Concrete GPU: Integrated Graphics
class IntegratedGPU : public GPU {
public:
    IntegratedGPU(const string &model = "Intel UHD 770")
        : GPU(model, "Integrated", 0, 1500, 32, 15) {}

    IntegratedGPU *clone() const override
    {
        return new IntegratedGPU(*this);
    }
};
