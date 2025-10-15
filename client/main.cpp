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
    Power8Pin* cable1 = new Power8Pin(150);
    Power8Pin* cable2 = new Power8Pin(150);
    Power12Pin* adapter = new DualPower8PinTo12PinAdapter(cable1, cable2);
    cout << adapter->connect() << endl;
    cout << "Total: " << adapter->getPowerRating() << "W" << endl;

    // ============================================================
    // FLYWEIGHT PATTERN: Demonstrate shared RAM specs
    // ============================================================
    cout << "\n--- FLYWEIGHT PATTERN ---" << endl;
    cout << "Adding more RAM modules with shared specs:" << endl;

    RAMSpecFactory specFactory;
    auto ddr5_6000_cl30 = specFactory.getRAMSpec("DDR5", 6000, 30);
    auto ddr5_6000_cl30_reuse = specFactory.getRAMSpec("DDR5", 6000, 30);

    RAM ram1(ddr5_6000_cl30, 16, "Corsair");
    RAM ram2(ddr5_6000_cl30_reuse, 16, "Kingston");

    gamingPC->addRAM(&ram1);
    gamingPC->addRAM(&ram2);

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

    cout << "\n========== Demo Complete ==========" << endl;
    return 0;
}
