#pragma once
#include "../Component.h"

class PrototypeFactory {
private:
    unordered_map<string, Component*> prototypes;

public:
    void registerPrototype(const string& key, Component* prototype) {
        prototypes[key] = prototype;
    }

    Component* create(const string& key) {
        if (prototypes.find(key) != prototypes.end())
            return prototypes[key]->clone();
        else
            return nullptr;
    }

    ~PrototypeFactory() {
        for (auto& p : prototypes)
            delete p.second;
    }
};