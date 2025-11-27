#include "BaseclassVehicle.h"

class Moto : public Vehicle {
    int power;
    std::string moto_name;
public:
    Moto(int pw, std::string nm) : Vehicle(2, "Moto") {
        power = pw;
        moto_name = nm;
    }
    void drive() override {
        std::cout << "Moto is driving\n";
    }
    void stop() override {
        std::cout << "Moto stopped\n";
    }
    void print_power() const {
        std::cout << moto_name << " Have " <<
            power << " horse powers\n";
    }
    virtual void break_moto() = 0;
};
