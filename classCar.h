#include "BaseclassVehicle.h"

class Car : public Vehicle {
public:
    void drive() override {
        std::cout << "Car is driving\n";
    }
    void stop() override {
        std::cout << "Car stopped\n";
    }
    virtual void ride_10_minutes() = 0;
    virtual void refuel() = 0;
};
