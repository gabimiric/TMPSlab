#pragma once
#include "../PC.h"
#include "../factory/PrototypeFactory.h"

class PCBuilder {
protected:
    PC* pc;
    PrototypeFactory* factory;
public:
    PCBuilder(PrototypeFactory* f) : factory(f), pc(nullptr) {}
    virtual ~PCBuilder() {}

    virtual void buildCPU() = 0;
    virtual void buildGPU() = 0;
    virtual void buildRAM() = 0;

    PC* getPC() { return pc; }
};

class GamingPCBuilder : public PCBuilder {
public:
    GamingPCBuilder(PrototypeFactory* f) : PCBuilder(f) { pc = new PC("Gaming PC"); }
    void buildCPU() override { pc->addComponent(factory->create("HighEndCPU")); }
    void buildGPU() override { pc->addComponent(factory->create("HighEndGPU")); }
    void buildRAM() override { pc->addComponent(factory->create("LargeRAM")); }
};

class OfficePCBuilder : public PCBuilder {
public:
    OfficePCBuilder(PrototypeFactory* f) : PCBuilder(f) { pc = new PC("Office PC"); }
    void buildCPU() override { pc->addComponent(factory->create("MidCPU")); }
    void buildGPU() override { pc->addComponent(factory->create("IntegratedGPU")); }
    void buildRAM() override { pc->addComponent(factory->create("MediumRAM")); }
};

class AIPCBuilder : public PCBuilder {
public:
    AIPCBuilder(PrototypeFactory* f) : PCBuilder(f) { pc = new PC("AI Training PC"); }
    void buildCPU() override { pc->addComponent(factory->create("HighEndCPU")); }
    void buildGPU() override { pc->addComponent(factory->create("AIGPU")); }
    void buildRAM() override { pc->addComponent(factory->create("LargeRAM")); }
};
