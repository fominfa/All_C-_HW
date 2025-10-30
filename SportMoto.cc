#include "classMoto.h"

class Sport : public Moto {
public:
    Sport() : Moto(100, "sport moto") {}

    void print_sport() const { "I can ride very fast\n"; }

    void break_moto() override {
        std::cout << "I have been broken\n";
        this->~Sport();
    }
};
