#pragma once
#include <map>
#include <memory>
#include <string>
#include <iostream>
#include "../models/RAM.hpp"
using namespace std;

// FLYWEIGHT PATTERN: Factory for shared RAM specifications
class RAMSpecFactory {
private:
    map<string, shared_ptr<RAMSpec> > specPool;

    string createKey(const string &ddrType, int transferRate, int clRating)
    {
        return ddrType + "_" + to_string(transferRate) + "_" + to_string(clRating);
    }

public:
    shared_ptr<RAMSpec> getRAMSpec(const string &ddrType, int transferRate, int clRating)
    {
        string key = createKey(ddrType, transferRate, clRating);

        if (specPool.find(key) == specPool.end())
        {
            cout << "  Creating spec: " << key << endl;
            specPool[key] = make_shared<RAMSpec>(ddrType, transferRate, clRating);
        }
        else
        {
            cout << "  Reusing spec: " << key << endl;
        }

        return specPool[key];
    }

    int getPoolSize() const { return specPool.size(); }
};
