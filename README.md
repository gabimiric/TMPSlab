# Tehnici și mecanisme de proiectare software

### Lab 1 - Design Patterns

### Author: Gabriel Miricinschi

------

## Objectives:

1. Study and understand the Creational Design Patterns.
2. Choose a domain, define its main classes/models/entities and choose the appropriate instantiation mechanisms.
3. Use some creational design patterns for object instantiation in a sample project.

## Used Design Patterns:

1. Singleton - Ensures only one instance of a class exists and provides a global point of access to it.
2. Builder - Separates the construction of a complex object from its representation, allowing step-by-step creation.
3. Prototype - Creates new objects by copying existing ones, rather than instantiating from scratch.

## Implementation

This project is a simulation of an Employee building PCs.

```c++
class GPU {
protected:
    string model;
    string cooler;
    int vramGB;
    int clockSpeedMHz;
    int cores;
public:
    GPU(const string& m, const string& c, int v, int clock, int cores) 
        : model(m), cooler(c), vramGB(v), clockSpeedMHz(clock), cores(cores) {}
    virtual ~GPU() = default;
    virtual GPU* clone() const = 0;
    virtual void show() const {
        cout << "GPU: " << model 
             << " | Cooler: " << cooler 
             << " | VRAM: " << vramGB << "GB"
             << " | Clock: " << clockSpeedMHz << " MHz"
             << " | Cores: " << cores << endl;
    }
    void setCooler(const string& c) { cooler = c; }
    void setVRAM(int v) { vramGB = v; }
    void setClockSpeed(int clock) { clockSpeedMHz = clock; }
    void setCores(int cores) { cores = cores; }
};

class RTX3080 : public GPU {
public:
    RTX3080(const string& cooler = "Stock", int vram = 10, int clock = 1710, int cores = 8704)
        : GPU("NVIDIA RTX 3080", cooler, vram, clock, cores) {}
    RTX3080* clone() const override {
        return new RTX3080(*this);
    }
};
```

The `GPU` classes are made through _Prototype Design Pattern_. I can add new GPUs that extend the base class, with `clone()` being the 
method that clones a new object from an existing one.

```c++
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
        RTX4090 prototypeGPU("Liquid Cooled", 24, 2600, 16384);
        pc->setGPU(prototypeGPU.clone());
    }
    void buildRAM() override { 
        pc->setRAM("64GB DDR5");
    }
};
```

The `PCBuilder` class is made through _Builder Design Pattern_. It is used to build a PC object step by step by implementing extensions 
of the class that build different PCs with differing specifications.

```c++
class EmployeeManager {
private:
    static EmployeeManager* instance;
    string name;
    int pcsBuilt;
    vector<string> buildLog;
    EmployeeManager(const string& n) : name(n), pcsBuilt(0) {
        cout << "[System] Employee Manager " << name << " has started their shift.\n";
    }
public:
    EmployeeManager(const EmployeeManager&) = delete;
    EmployeeManager& operator=(const EmployeeManager&) = delete;
    static EmployeeManager* getInstance(const string& n = "Default Manager") {
        if (instance == nullptr) {
            instance = new EmployeeManager(n);
        }
        return instance;
    }
    //Other public methods
}
```

Lastly, the `EmployeeManager` class is made through _Singleton Design Pattern_. It is a simulation of an employee/manager that can build PCs and
can only have one instance at a time.

```c++
int main() {
    cout << "=== Design Patterns Demo ===" << endl;
    EmployeeManager* manager = EmployeeManager::getInstance("Alice Johnson");
    manager->showInfo();
    EmployeeManager* manager1 = EmployeeManager::getInstance("Bob Smith");
    manager1->showInfo();
    cout << "\n[PROTOTYPE PATTERN]" << endl;
    RTX3080 baseGPU("Stock", 10, 1710, 8704);
    RTX3080* customGPU = baseGPU.clone();
    customGPU->setCooler("Water Cooled");
    customGPU->setClockSpeed(1950);
    manager->validateGPU(customGPU);
    delete customGPU;
    cout << "\n[BUILDER PATTERN]" << endl;
    Director director;
    HighEndPCBuilder builder;
    PC* gamingPC = manager->buildPC(director, builder, "Full");
    gamingPC->showSpecs();
    manager->showBuildSummary();
    delete gamingPC;
    return 0;
}
```

This is the `main` code used for testing. It shows the Singleton, Prototype and Builder design patterns in action.

The **Singleton** test will show that the manager can only have one instance at a time. 

The **Prototype** test will show that the GPU can be cloned and modified. 

The **Builder** test will show that the PC can be built step by step.

## Results & Screenshots

<img width="731" height="750" alt="Image" src="https://github.com/user-attachments/assets/f1452904-e9ec-4155-91e7-bb4afbedb37c" />