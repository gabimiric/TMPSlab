#include <iostream>
#include "../domain/models/Employee.h"
#include "../domain/models/GPU.h"
#include "../domain/factory/PCBuilder.h"
#include "../domain/factory/Director.h"
using namespace std;

int main() {
    cout << "=== Design Patterns Demo ===" << endl;
    
    // SINGLETON: Only one manager exists and can't create another one
    EmployeeManager* manager = EmployeeManager::getInstance("Alice Johnson");
    manager->showInfo();

    EmployeeManager* manager1 = EmployeeManager::getInstance("Bob Smith");
    manager1->showInfo();

    // PROTOTYPE: Clone and customize GPU
    cout << "\n[PROTOTYPE PATTERN]" << endl;
    RTX3080 baseGPU("Stock", 10, 1710, 8704);
    RTX3080* customGPU = baseGPU.clone();
    customGPU->setCooler("Water Cooled");
    customGPU->setClockSpeed(1950);
    manager->validateGPU(customGPU);
    delete customGPU;

    // BUILDER: Manager builds PC using Director and Builder
    cout << "\n[BUILDER PATTERN]" << endl;
    Director director;
    HighEndPCBuilder builder;
    PC* gamingPC = manager->buildPC(director, builder, "Full");
    gamingPC->showSpecs();

    // Summary and cleanup
    manager->showBuildSummary();
    delete gamingPC;

    return 0;
}