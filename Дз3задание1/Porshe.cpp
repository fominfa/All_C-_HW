#include "classCar.h"

class Porche : public Car {
public:
    Porche() : Car(400, "Toyota", 1200) {}
    void ride_10_minutes() override {
        std::cout << "Porche drove " << std::min(reserve, 40)
            << " kilometrs\n";
        reserve = std::max(reserve - 40, 0);
    }
    void refuel() override {
        std::cout << "Porche has been refueled\n";
        reserve = 1200;
    }
    void print_Porche() const { std::cout << "I am a premium car\n"; }
};
