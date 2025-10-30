#include "classBus.h"

class MiniBus : public Bus {
public:
    MiniBus() : Bus(15, "MiniBus", 0) {}
    void seat_passengers(int c) {
        if (now_pass + c > 15) {
            std::cout << "Not enough places for all\n";
        }
        now_pass = std::min(now_pass + c, 15);
    }
};
