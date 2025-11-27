#include "classCar.h"

class BMW : public Car {
public:
    BMW() : Car(250, "BMW", 1000) {}
    void ride_10_minutes() override {
        std::cout << "BMW drove " << std::min(reserve, 20)
            << " kilometrs\n";
        reserve = std::max(reserve - 20, 0);
    }
    void refuel() override {
        std::cout << "BMW has been refueled\n";
        reserve = 1000;
    }
};
