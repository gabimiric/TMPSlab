#pragma once
#include <vector>
#include "../PC.h"
#include "../models/GPU.h"
#include "../models/RAM.hpp"
#include "RAMSpecFactory.hpp"
using namespace std;

class PCBuilder {
protected:
    PC *pc;

public:
    PCBuilder() : pc(nullptr) {}
    virtual ~PCBuilder() {}

    virtual void buildCPU() = 0;
    virtual void buildGPU() = 0;
    virtual void buildRAM() = 0;

    PC *getPC() { return pc; }
};

class HighEndPCBuilder : public PCBuilder {
private:
    RAMSpecFactory specFactory;
    vector<RAM*> ramStorage;

public:
    HighEndPCBuilder() { pc = new PC("Gaming PC"); }

    void buildCPU() override
    {
        pc->setCPU("AMD Ryzen 7 9800X3D");
    }

    void buildGPU() override
    {
        // Create a prototype RTX 4090 with custom config
        RTX4090 prototypeGPU("Liquid Cooled", 24, 2600, 16384);
        // Clone it for this PC
        pc->setGPU(prototypeGPU.clone());
    }

    void buildRAM() override
    {
        // Use Flyweight pattern to share RAM specs
        auto ddr5_6000_cl30 = specFactory.getRAMSpec("DDR5", 6000, 30);

        // High-end gaming PC gets 2x32GB modules
        RAM* ram1 = new RAM(ddr5_6000_cl30, 32, "G.Skill Trident Z5");
        RAM* ram2 = new RAM(ddr5_6000_cl30, 32, "G.Skill Trident Z5");

        ramStorage.push_back(ram1);
        ramStorage.push_back(ram2);

        pc->addRAM(ram1);
        pc->addRAM(ram2);
    }

    ~HighEndPCBuilder() {
        for (auto ram : ramStorage) {
            delete ram;
        }
    }
};

class OfficePCBuilder : public PCBuilder {
private:
    RAMSpecFactory specFactory;
    vector<RAM*> ramStorage;

public:
    OfficePCBuilder() { pc = new PC("Office PC"); }

    void buildCPU() override
    {
        pc->setCPU("Intel i5-13600K");
    }

    void buildGPU() override
    {
        // Use integrated graphics
        IntegratedGPU prototypeGPU;
        pc->setGPU(prototypeGPU.clone());
    }

    void buildRAM() override
    {
        auto ddr4_3200_cl16 = specFactory.getRAMSpec("DDR4", 3200, 16);

        // Office PC gets 2x8GB modules
        RAM* ram1 = new RAM(ddr4_3200_cl16, 8, "Corsair Vengeance");
        RAM* ram2 = new RAM(ddr4_3200_cl16, 8, "Corsair Vengeance");

        ramStorage.push_back(ram1);
        ramStorage.push_back(ram2);

        pc->addRAM(ram1);
        pc->addRAM(ram2);
    }

    ~OfficePCBuilder() {
        for (auto ram : ramStorage) {
            delete ram;
        }
    }
};

class BudgetGamingPCBuilder : public PCBuilder {
private:
    RAMSpecFactory specFactory;
    vector<RAM*> ramStorage;

public:
    BudgetGamingPCBuilder() { pc = new PC("Budget Gaming PC"); }

    void buildCPU() override
    {
        pc->setCPU("AMD Ryzen 5 5600X");
    }

    void buildGPU() override
    {
        // Prototype with custom cooler and overclocked
        RX6600 prototypeGPU("Triple Fan", 8, 2700, 1792);
        pc->setGPU(prototypeGPU.clone());
    }

    void buildRAM() override
    {
        auto ddr4_3200_cl16 = specFactory.getRAMSpec("DDR4", 3200, 16);

        // Budget gaming PC gets 2x8GB modules
        RAM* ram1 = new RAM(ddr4_3200_cl16, 8, "Kingston FURY");
        RAM* ram2 = new RAM(ddr4_3200_cl16, 8, "Kingston FURY");

        ramStorage.push_back(ram1);
        ramStorage.push_back(ram2);

        pc->addRAM(ram1);
        pc->addRAM(ram2);
    }

    ~BudgetGamingPCBuilder() {
        for (auto ram : ramStorage) {
            delete ram;
        }
    }
};

class GamingPCBuilder : public PCBuilder {
private:
    RAMSpecFactory specFactory;
    vector<RAM*> ramStorage;

public:
    GamingPCBuilder() { pc = new PC("High-End Gaming PC"); }

    void buildCPU() override
    {
        pc->setCPU("Intel i9-14900K");
    }

    void buildGPU() override
    {
        // Create base prototype
        RTX3080 basePrototype("Stock", 10, 1710, 8704);

        // Clone and customize with overclock
        RTX3080 *customGPU = basePrototype.clone();
        customGPU->setVRAM(12); // 12GB variant
        customGPU->setClockSpeed(1950); // Overclocked

        pc->setGPU(customGPU);
    }

    void buildRAM() override
    {
        auto ddr4_3600_cl18 = specFactory.getRAMSpec("DDR4", 3600, 18);

        // Gaming PC gets 2x16GB modules
        RAM* ram1 = new RAM(ddr4_3600_cl18, 16, "Corsair Vengeance RGB");
        RAM* ram2 = new RAM(ddr4_3600_cl18, 16, "Corsair Vengeance RGB");

        ramStorage.push_back(ram1);
        ramStorage.push_back(ram2);

        pc->addRAM(ram1);
        pc->addRAM(ram2);
    }

    ~GamingPCBuilder() {
        for (auto ram : ramStorage) {
            delete ram;
        }
    }
};
