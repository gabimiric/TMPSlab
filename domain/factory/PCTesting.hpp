#pragma once
#include <iostream>
#include "../PC.h"
using namespace std;

// Subsystem 1: BIOS operations
class BIOSSystem {
public:
    bool runPOST(PC* pc) {
        cout << "  [BIOS] Power-On Self-Test initiated..." << endl;

        bool cpuPresent = pc->hasCPU();
        bool gpuPresent = pc->hasGPU();
        bool ramPresent = pc->hasRAM();

        cout << "  [BIOS] CPU: " << (cpuPresent ? "✓" : "✗") << endl;
        cout << "  [BIOS] GPU: " << (gpuPresent ? "✓" : "⚠") << endl;
        cout << "  [BIOS] RAM: " << (ramPresent ? "✓" : "✗") << endl;

        return cpuPresent && ramPresent;
    }

    void initializePower() {
        cout << "  [BIOS] PSU: OK | Motherboard: OK" << endl;
    }
};

// Subsystem 2: Hardware monitoring
class HardwareMonitor {
public:
    void checkTemperatures(bool hasGPU) {
        cout << "  [HWMonitor] CPU: 45°C (idle) | Max: 72°C" << endl;
        if (hasGPU) {
            cout << "  [HWMonitor] GPU: 42°C (idle) | Max: 68°C" << endl;
        }
    }

    void runStressTest(bool hasGPU) {
        cout << "  [Stress] CPU burn-in: Load 100% | Temp: 72°C ✓" << endl;
        if (hasGPU) {
            cout << "  [Stress] GPU benchmark: FPS 144 | Temp: 68°C ✓" << endl;
        }
        cout << "  [Stress] Memory test: 0 errors ✓" << endl;
    }
};

// Subsystem 3: Diagnostic logger
class DiagnosticLogger {
public:
    void generateReport(const string& pcName, bool healthy) {
        cout << "  ---" << endl;
        cout << "  System: " << pcName << endl;

        if (healthy) {
            cout << "  Status: ✓ OPERATIONAL" << endl;
        } else {
            cout << "  Status: ✗ FAILED" << endl;
        }

        cout << "  ---" << endl;
    }
};

// FACADE: Simplifies interaction with multiple subsystems
class PCTesting {
private:
    PC* pc;
    BIOSSystem bios;
    HardwareMonitor hwMonitor;
    DiagnosticLogger logger;
    bool systemHealthy;

public:
    PCTesting(PC* pc) : pc(pc), systemHealthy(true) {}

    void runFullDiagnostics() {
        cout << "Running diagnostics on: " << pc->getName() << endl;

        // Coordinate multiple subsystems
        bios.initializePower();

        if (!bios.runPOST(pc)) {
            systemHealthy = false;
            logger.generateReport(pc->getName(), systemHealthy);
            return;
        }

        hwMonitor.runStressTest(pc->hasGPU());
        hwMonitor.checkTemperatures(pc->hasGPU());

        logger.generateReport(pc->getName(), systemHealthy);
    }

    void quickTest() {
        bios.initializePower();

        if (bios.runPOST(pc)) {
            cout << "✓ System can boot" << endl;
        } else {
            cout << "✗ System cannot boot" << endl;
        }
    }
};