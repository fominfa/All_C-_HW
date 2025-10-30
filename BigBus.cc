#include "classBus.h"

class BigBus : public Bus {
public:
    BigBus() : Bus(40, "MiniBus", 0) {}
    void seat_passengers(int c) {
        if (now_pass + c > 40) {
            std::cout << "Not enough places for all\n";
        }
        now_pass = std::min(now_pass + c, 40);
    }
};
