#pragma once
#include <string>
using namespace std;

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
    int getPower() const { return powerRating; }
};

// ADAPTER PATTERN: Converts two 8-pin to 12-pin
class DualPower8PinTo12PinAdapter : public Power12Pin {
private:
    Power8Pin* cable1;
    Power8Pin* cable2;
public:
    DualPower8PinTo12PinAdapter(Power8Pin* c1, Power8Pin* c2)
        : cable1(c1), cable2(c2) {}

    string connect() override {
        return "Combining 2x 8-pin -> 12-pin (" +
               to_string(cable1->getPower() + cable2->getPower()) + "W total)";
    }

    int getPowerRating() const override {
        return cable1->getPower() + cable2->getPower();
    }

    ~DualPower8PinTo12PinAdapter() {
        delete cable1;
        delete cable2;
    }
};
