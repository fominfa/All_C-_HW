#include "BaseclassVehicle.h"

class Car : public Vehicle {
public:
    int power;
    std::string car_name;
    int reserve;

    Car(int pw, std::string nm, int rs) : Vehicle(4, "Car") {
        power = pw;
        car_name = nm;
        reserve = rs;
    }

    void drive() override {
        std::cout << "Car is driving\n";
    }
    void stop() override {
        std::cout << "Car stopped\n";
    }
    void print_power() const {
        std::cout << car_name << " Have " <<
            power << " horse powers\n";
    }
    void print_reserve() const {
        std::cout << "There is enough gasoline to "
            << reserve << " kilometrs\n";
    }
    virtual void ride_10_minutes() = 0;
    virtual void refuel() = 0;
};
