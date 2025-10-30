#include "BaseclassVehicle.h"

class Bus : public Vehicle {
    int passanger;
    std::string bus_name;
public:
    int now_pass;
    Bus(int c, std::string nm, int np) : Vehicle(8, "Bus") {
        passanger = c;
        bus_name = nm;
        now_pass = np;
    }
    void drive() override {
        std::cout << "Bus is driving\n";
    }
    void stop() override {
        std::cout << "Bus stopped\n";
    }
    void print_accomadation() const {
        std::cout << bus_name << " can accomodate " << passanger << " people\n";
    }
    virtual void seat_passengers() = 0;
    void drop_passengers() {
        if (now_pass == 0) {
            std::cout << "There is no passengers in the bus\n";
        }
        now_pass = 0;
    }
};
