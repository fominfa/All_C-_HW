#include <iostream>
#include <string>

class Vehicle {
    int wheel_count;
    std::string name;
public:
    Vehicle(int c, std::string vehicle_name) {
        wheel_count = c;
        name = vehicle_name;
    }
    virtual void drive() = 0;
    virtual void stop() = 0;
    void print_wheels() const { std::cout << name << " have "
        << wheel_count << " wheels"; }
};
