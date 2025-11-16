#include <iostream>
#include "../domain/models/Employee.h"
#include "../domain/models/PowerCable.hpp"
#include "../domain/models/RAM.hpp"
#include "../domain/factory/PCBuilder.h"
#include "../domain/factory/Director.h"
#include "../domain/factory/RAMSpecFactory.hpp"
#include "../domain/factory/PCTesting.hpp"
using namespace std;

int main()
{
    cout << "========== PC Building Demo ==========" << endl;

    // Build a gaming PC
    Director director;
    HighEndPCBuilder builder;
    director.constructFullPC(builder);
    PC* gamingPC = builder.getPC();

    gamingPC->showSpecs();
    gamingPC->showRAMInfo();

    // ============================================================
    // ADAPTER PATTERN: 8-pin to 12-pin power adapter
    // ============================================================
    cout << "\n--- ADAPTER PATTERN ---" << endl;
    cout << "Problem: GPU requires 12-pin power, but PSU only has 8-pin cables" << endl;

    Power8Pin* cable1 = new Power8Pin(300);
    Power8Pin* cable2 = new Power8Pin(300);
    Power12Pin* adapter = new DualPower8PinTo12PinAdapter(cable1, cable2);

    // Actually connect it to the GPU
    GPU* gpu = gamingPC->getGPU();
    if (gpu) {
        cout << "\nConnecting " << cable1->getPowerRating() << "W to "
             << gpu->getName() << " (requires " << gpu->getPowerConsumption() << "W)" << endl;

        if (cable1->getPowerRating() >= gpu->getPowerConsumption()) {
            cout << "✓ GPU powered successfully!" << endl;
        } else {
            cout << "✗ Insufficient power for GPU!" << endl;
        }
    }

    cout << "\nSolution: Using adapter to connect power to GPU" << endl;
    cout << adapter->connect() << endl;
    cout << "Total power available: " << adapter->getPowerRating() << "W" << endl;

    // Connect it to the GPU again
    if (gpu) {
        cout << "\nConnecting " << adapter->getPowerRating() << "W to "
             << gpu->getName() << " (requires " << gpu->getPowerConsumption() << "W)" << endl;

        if (adapter->getPowerRating() >= gpu->getPowerConsumption()) {
            cout << "✓ GPU powered successfully!" << endl;
        } else {
            cout << "✗ Insufficient power for GPU!" << endl;
        }
    }

    // ============================================================
    // FLYWEIGHT PATTERN: Demonstrate shared RAM specs
    // ============================================================
    cout << "\n--- FLYWEIGHT PATTERN ---" << endl;
    cout << "Adding more RAM modules with shared specs:" << endl;

    RAMSpecFactory specFactory;
    auto ddr5_6000_cl30 = specFactory.getRAMSpec("DDR5", 6000, 30);

    RAM ram1(ddr5_6000_cl30, 16, "Corsair");
    RAM ram2(ddr5_6000_cl30, 16, "Kingston");

    gamingPC->addRAM(&ram1);
    gamingPC->addRAM(&ram2);

    gamingPC->showRAMInfo();

    cout << "Total RAM modules: " << gamingPC->getRAMModuleCount() << endl;
    cout << "Unique specs in pool: " << specFactory.getPoolSize() << endl;

    // ============================================================
    // FACADE PATTERN: Simplified PC testing
    // ============================================================
    cout << "\n--- FACADE PATTERN ---" << endl;
    PCTesting tester(gamingPC);
    tester.runFullDiagnostics();

    // Cleanup
    delete gamingPC;
    delete adapter;


    return 0;
}
