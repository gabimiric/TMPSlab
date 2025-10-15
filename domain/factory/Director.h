#pragma once
#include "PCBuilder.h"
#include <iostream>
using namespace std;

class Director {
public:
    // Standard full build - all components
    void constructFullPC(PCBuilder& builder) {
        cout << "Building PC..." << endl;
        builder.buildCPU();
        builder.buildGPU();
        builder.buildRAM();
    }
    
    // Quick build - skip GPU (for servers or workstations with integrated graphics)
    void constructBasicPC(PCBuilder& builder) {
        cout << "Director: Building basic PC (no dedicated GPU)..." << endl;
        builder.buildCPU();
        builder.buildRAM();
        // No GPU - using integrated graphics
    }
    
    // GPU-focused build - build GPU first with emphasis
    void constructGraphicsPC(PCBuilder& builder) {
        cout << "Director: Building graphics-focused PC..." << endl;
        builder.buildGPU();  // GPU first - most important
        builder.buildCPU();
        builder.buildRAM();
    }
    
    // Minimal build - just essentials in priority order
    void constructMinimalPC(PCBuilder& builder) {
        cout << "Director: Building minimal viable PC..." << endl;
        builder.buildCPU();
        builder.buildRAM();
        // Skip GPU if not critical
    }
    
    // Step-by-step guided build with user feedback
    void constructCustomPC(PCBuilder& builder, bool includeGPU = true) {
        cout << "Director: Starting custom build process..." << endl;
        
        cout << "  Step 1/3: Installing processor..." << endl;
        builder.buildCPU();
        
        cout << "  Step 2/3: Installing memory..." << endl;
        builder.buildRAM();
        
        if (includeGPU) {
            cout << "  Step 3/3: Installing graphics card..." << endl;
            builder.buildGPU();
        } else {
            cout << "  Step 3/3: Skipping GPU (using integrated)..." << endl;
        }
        
        cout << "Director: Build complete!" << endl;
    }
};