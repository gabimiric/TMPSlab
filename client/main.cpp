#include <iostream>
#include "../domain/models/Employee.h"
#include "../domain/factory/PrototypeFactory.h"
#include "../domain/factory/PCBuilder.h"
#include "../domain/factory/Director.h"
using namespace std;

int main() {
    // Singleton
    EmployeeManager* manager = EmployeeManager::getInstance("Alice Johnson");
    manager->showInfo();

    // Prototype setup
    PrototypeFactory factory;
    factory.registerPrototype("HighEndCPU", new CPU("Intel i9-14900K"));
    factory.registerPrototype("MidCPU", new CPU("Intel i5-13600K"));
    factory.registerPrototype("HighEndGPU", new GPU("RTX 4090"));
    factory.registerPrototype("IntegratedGPU", new GPU("Intel UHD 770"));
    factory.registerPrototype("AIGPU", new GPU("NVIDIA H100"));
    factory.registerPrototype("LargeRAM", new RAM("64GB DDR5"));
    factory.registerPrototype("MediumRAM", new RAM("32GB DDR4"));

    // Builder usage
    Director director;

    GamingPCBuilder gamingBuilder(&factory);
    director.constructPC(gamingBuilder);
    PC* gamingPC = gamingBuilder.getPC();
    gamingPC->showSpecs();

    OfficePCBuilder officeBuilder(&factory);
    director.constructPC(officeBuilder);
    PC* officePC = officeBuilder.getPC();
    officePC->showSpecs();

    AIPCBuilder aiBuilder(&factory);
    director.constructPC(aiBuilder);
    PC* aiPC = aiBuilder.getPC();
    aiPC->showSpecs();

    // Cleanup
    delete gamingPC;
    delete officePC;
    delete aiPC;

    return 0;
}