#include "classCar.h"

class Shkoda : public Car {
public:
    Shkoda() : Car(180, "Shkoda", 900) {}
    void ride_10_minutes() override {
        std::cout << "Shkoda drove " << std::min(reserve, 17)
            << " kilometrs\n";
        reserve = std::max(reserve - 17, 0);
    }
    void refuel() override {
        std::cout << "Shkoda has been refueled\n";
        reserve = 900;
    }
};
