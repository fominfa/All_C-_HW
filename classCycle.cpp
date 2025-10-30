#include "BaseclassVehicle.h"

class Cycle : public Vehicle {
    Cycle() : Vehicle(2, "Cycle") {}
    void drive() override {
        std::cout << "Cycle is driving\n";
    }
    void stop() override {
        std::cout << "Cycle stopped\n";
    }
};
