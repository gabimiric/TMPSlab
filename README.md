# Tehnici și mecanisme de proiectare software

### Lab 2 - Structural Design Patterns

### Author: Gabriel Miricinschi

------

## Objectives:

1. Study and understand the Structural Design Patterns.
2. Extend the existing domain with structural patterns to improve code organization and flexibility.
3. Implement at least 3 structural design patterns in the sample project.

## Used Design Patterns:

### Structural Patterns (Lab 2):
1. **Adapter** - Converts one interface into another, allowing incompatible interfaces to work together.
2. **Flyweight** - Reduces memory usage by sharing common data between similar objects.
3. **Facade** - Provides a simplified interface to a complex subsystem.

## Implementation

This project extends the PC building simulation from Lab 1 with structural design patterns to handle compatibility issues, optimize memory usage, and simplify complex operations.

### 1. Adapter Pattern - Power Cable Compatibility

The **Adapter Pattern** solves the problem of incompatible power connectors. Modern high-end GPUs require 12-pin power connectors, but older PSUs only have 8-pin cables.

```c++
// Target interface (12-pin)
class Power12Pin {
public:
    virtual ~Power12Pin() = default;
    virtual string connect() = 0;
    virtual int getPowerRating() const = 0;
};

// Adaptee (8-pin)
class Power8Pin {
private:
    int powerRating;
public:
    Power8Pin(int powerRating) : powerRating(powerRating) {}
    string connect8Pin() {
        return "8-pin connector (" + to_string(powerRating) + "W)";
    }
    int getPowerRating() const { return powerRating; }
};

// ADAPTER: Converts two 8-pin to 12-pin
class DualPower8PinTo12PinAdapter : public Power12Pin {
private:
    Power8Pin* cable1;
    Power8Pin* cable2;
public:
    DualPower8PinTo12PinAdapter(Power8Pin* c1, Power8Pin* c2)
        : cable1(c1), cable2(c2) {}

    string connect() override {
        return "Combining 2x 8-pin -> 12-pin (" +
               to_string(cable1->getPowerRating() + cable2->getPowerRating()) + "W total)";
    }

    int getPowerRating() const override {
        return cable1->getPowerRating() + cable2->getPowerRating();
    }
};
```

The adapter allows two 8-pin cables to be combined into a single 12-pin connection, providing enough power for modern GPUs while maintaining compatibility with older power supplies.

### 2. Flyweight Pattern - RAM Specification Sharing

The **Flyweight Pattern** optimizes memory usage when dealing with multiple RAM modules that share common specifications (DDR type, transfer rate, CL rating).

```c++
// Intrinsic state (shared)
class RAMSpec {
private:
    string ddrType;
    int transferRate;
    int clRating;
public:
    RAMSpec(const string& ddrType, int transferRate, int clRating)
        : ddrType(ddrType), transferRate(transferRate), clRating(clRating) {}
    
    string getDDRType() const { return ddrType; }
    int getTransferRate() const { return transferRate; }
    int getCLRating() const { return clRating; }
};

// Extrinsic state (unique per module)
class RAM {
private:
    shared_ptr<RAMSpec> specs;  // Shared flyweight
    int capacityGB;
    string brand;
public:
    RAM(shared_ptr<RAMSpec> specs, int capacityGB, const string& brand)
        : specs(specs), capacityGB(capacityGB), brand(brand) {}
    
    void showInfo() const {
        cout << "  " << brand << " " << capacityGB << "GB "
             << specs->getDDRType() << "-" << specs->getTransferRate()
             << " CL" << specs->getCLRating() << endl;
    }
};

// Flyweight Factory
class RAMSpecFactory {
private:
    map<string, shared_ptr<RAMSpec>> specPool;
public:
    shared_ptr<RAMSpec> getRAMSpec(const string& ddrType, int transferRate, int clRating) {
        string key = ddrType + "_" + to_string(transferRate) + "_" + to_string(clRating);
        
        if (specPool.find(key) == specPool.end()) {
            specPool[key] = make_shared<RAMSpec>(ddrType, transferRate, clRating);
        }
        return specPool[key];
    }
};
```

Multiple RAM modules with the same specifications (e.g., DDR5-6000 CL30) share a single `RAMSpec` object, reducing memory overhead when building systems with multiple identical RAM sticks.

### 3. Facade Pattern - Simplified PC Diagnostics

The **Facade Pattern** provides a simplified interface to the complex subsystems involved in PC testing: BIOS operations, hardware monitoring, and diagnostic logging.

```c++
// Subsystem 1: BIOS operations
class BIOSSystem {
public:
    bool runPOST(PC* pc) {
        // Power-On Self-Test checks
        bool cpuPresent = pc->hasCPU();
        bool ramPresent = pc->hasRAM();
        return cpuPresent && ramPresent;
    }
    void initializePower() { /* ... */ }
};

// Subsystem 2: Hardware monitoring
class HardwareMonitor {
public:
    void checkTemperatures(bool hasGPU) { /* ... */ }
    void runStressTest(bool hasGPU) { /* ... */ }
};

// Subsystem 3: Diagnostic logger
class DiagnosticLogger {
public:
    void generateReport(const string& pcName, bool healthy) { /* ... */ }
};

// FACADE: Simplifies complex diagnostic operations
class PCTesting {
private:
    PC* pc;
    BIOSSystem bios;
    HardwareMonitor hwMonitor;
    DiagnosticLogger logger;
public:
    PCTesting(PC* pc) : pc(pc) {}
    
    void runFullDiagnostics() {
        // Single method coordinates all subsystems
        bios.initializePower();
        if (!bios.runPOST(pc)) return;
        hwMonitor.runStressTest(pc->hasGPU());
        hwMonitor.checkTemperatures(pc->hasGPU());
        logger.generateReport(pc->getName(), true);
    }
};
```

Instead of interacting with multiple complex subsystems, clients can simply call `runFullDiagnostics()` to perform a complete PC test.

## Testing & Results

The main program demonstrates all three structural patterns in action:

```c++
int main() {
    // Build a gaming PC using creational patterns from Lab 1
    Director director;
    HighEndPCBuilder builder;
    director.constructFullPC(builder);
    PC* gamingPC = builder.getPC();
    
    // ADAPTER PATTERN: Power cable compatibility
    cout << "\n--- ADAPTER PATTERN ---" << endl;
    Power8Pin* cable1 = new Power8Pin(300);
    Power8Pin* cable2 = new Power8Pin(300);
    Power12Pin* adapter = new DualPower8PinTo12PinAdapter(cable1, cable2);
    
    cout << adapter->connect() << endl;
    cout << "Total power available: " << adapter->getPowerRating() << "W" << endl;
    
    // FLYWEIGHT PATTERN: Shared RAM specifications
    cout << "\n--- FLYWEIGHT PATTERN ---" << endl;
    RAMSpecFactory specFactory;
    auto ddr5_6000_cl30 = specFactory.getRAMSpec("DDR5", 6000, 30);
    
    RAM ram1(ddr5_6000_cl30, 16, "Corsair");
    RAM ram2(ddr5_6000_cl30, 16, "Kingston");
    gamingPC->addRAM(&ram1);
    gamingPC->addRAM(&ram2);
    
    cout << "Total RAM modules: " << gamingPC->getRAMModuleCount() << endl;
    cout << "Unique specs in pool: " << specFactory.getPoolSize() << endl;
    
    // FACADE PATTERN: Simplified PC testing
    cout << "\n--- FACADE PATTERN ---" << endl;
    PCTesting tester(gamingPC);
    tester.runFullDiagnostics();
    
    delete gamingPC;
    delete adapter;
    return 0;
}
```
