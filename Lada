#include "classCar.h"

class Lada : public Car {
public:
    Lada() : Car(120, "Lada", 700) {}
    void ride_10_minutes() override {
        std::cout << "Lada drove " << std::min(reserve, 10)
            << " kilometrs\n";
        reserve = std::max(reserve - 10, 0);
    }
    void refuel() override {
        std::cout << "Lada has been refueled\n";
        reserve = 700;
    }
};
