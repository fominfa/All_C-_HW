#include "classMoto.h"

class Tourist : public Moto {
public:
    Tourist() : Moto(100, "tourist moto") {}

    void print_tourist() const { "I can ride very in mountain\n"; }

    void break_moto() override {
        std::cout << "I have been broken in mountains\n";
        this->~Tourist();
    }
};
