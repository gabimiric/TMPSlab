#pragma once
#include "PCBuilder.h"

class Director {
public:
    void constructPC(PCBuilder& builder) {
        builder.buildCPU();
        builder.buildGPU();
        builder.buildRAM();
    }
};