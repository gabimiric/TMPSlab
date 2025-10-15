#pragma once
#include <string>
#include <memory>
#include <iostream>
using namespace std;

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

class RAM {
private:
    shared_ptr<RAMSpec> specs;
    int capacityGB;
    string brand;
public:
    RAM(shared_ptr<RAMSpec> specs, int capacityGB, const string& brand)
        : specs(specs), capacityGB(capacityGB), brand(brand) {}

    int getCapacity() const { return capacityGB; }  // Add this getter

    void showInfo() const {
        cout << "  " << brand << " " << capacityGB << "GB "
             << specs->getDDRType() << "-" << specs->getTransferRate()
             << " CL" << specs->getCLRating() << endl;
    }
};