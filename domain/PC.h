#pragma once
#include <iostream>
#include <vector>
#include "../domain/Component.h"
using namespace std;

class PC {
private:
    vector<Component*> components;
    string type;

public:
    PC(const string& t) : type(t) {}

    void addComponent(Component* c) {
        components.push_back(c);
    }

    void showSpecs() const {
        cout << "\n--- " << type << " Specifications ---" << endl;
        for (auto c : components)
            c->show();
        cout << "-------------------------------" << endl;
    }

    ~PC() {
        for (auto c : components)
            delete c;
    }
};