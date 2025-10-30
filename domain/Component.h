#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

// Base prototype class
class Component {
public:
    virtual ~Component() = default;
    virtual Component* clone() const = 0;
    virtual void show() const = 0;
};

// Concrete prototypes
class CPU : public Component {
    string model;
public:
    CPU(const string& m) : model(m) {}
    CPU* clone() const override { return new CPU(*this); }
    void show() const override { cout << "CPU: " << model << endl; }
};

class GPU : public Component {
    string model;
public:
    GPU(const string& m) : model(m) {}
    GPU* clone() const override { return new GPU(*this); }
    void show() const override { cout << "GPU: " << model << endl; }
};

class RAM : public Component {
    string size;
public:
    RAM(const string& s) : size(s) {}
    RAM* clone() const override { return new RAM(*this); }
    void show() const override { cout << "RAM: " << size << endl; }
};