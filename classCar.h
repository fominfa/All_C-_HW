#include "BaseclassVehicle.h"

class Car : public Vehicle {
    Car() : Vehicle(4, "Car") {}
    void drive() override {
        std::cout << "Car is driving\n";
    }
    void stop() override {
        std::cout << "Car stopped\n";
    }
};
