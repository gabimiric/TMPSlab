#pragma once
#include "../PC.h"
#include "../models/GPU.h"

class PCBuilder {
protected:
    PC* pc;
public:
    PCBuilder() : pc(nullptr) {}
    virtual ~PCBuilder() {}

    virtual void buildCPU() = 0;
    virtual void buildGPU() = 0;
    virtual void buildRAM() = 0;

    PC* getPC() { return pc; }
};

class HighEndPCBuilder : public PCBuilder {
public:
    HighEndPCBuilder() { pc = new PC("Gaming PC"); }
    
    void buildCPU() override { 
        pc->setCPU("AMD Ryzen 7 9800X3D");
    }
    
    void buildGPU() override { 
        // Create a prototype RTX 4090 with custom config
        RTX4090 prototypeGPU("Liquid Cooled", 24, 2600, 16384);
        // Clone it for this PC
        pc->setGPU(prototypeGPU.clone());
    }
    
    void buildRAM() override { 
        pc->setRAM("64GB DDR5");
    }
};

class OfficePCBuilder : public PCBuilder {
public:
    OfficePCBuilder() { pc = new PC("Office PC"); }
    
    void buildCPU() override { 
        pc->setCPU("Intel i5-13600K");
    }
    
    void buildGPU() override { 
        // Use integrated graphics
        IntegratedGPU prototypeGPU;
        pc->setGPU(prototypeGPU.clone());
    }
    
    void buildRAM() override { 
        pc->setRAM("16GB DDR4");
    }
};

class BudgetGamingPCBuilder : public PCBuilder {
public:
    BudgetGamingPCBuilder() { pc = new PC("Budget Gaming PC"); }
    
    void buildCPU() override { 
        pc->setCPU("AMD Ryzen 5 5600X");
    }
    
    void buildGPU() override { 
        // Prototype with custom cooler and overclocked
        RX6600 prototypeGPU("Triple Fan", 8, 2700, 1792);
        pc->setGPU(prototypeGPU.clone());
    }
    
    void buildRAM() override { 
        pc->setRAM("16GB DDR4");
    }
};

class GamingPCBuilder : public PCBuilder {
public:
    GamingPCBuilder() { pc = new PC("High-End Gaming PC"); }
    
    void buildCPU() override { 
        pc->setCPU("Intel i9-14900K");
    }
    
    void buildGPU() override { 
        // Create base prototype
        RTX3080 basePrototype("Stock", 10, 1710, 8704);
        
        // Clone and customize with overclock
        RTX3080* customGPU = basePrototype.clone();
        customGPU->setVRAM(12); // 12GB variant
        customGPU->setClockSpeed(1950); // Overclocked
        
        pc->setGPU(customGPU);
    }
    
    void buildRAM() override { 
        pc->setRAM("32GB DDR5");
    }
};
