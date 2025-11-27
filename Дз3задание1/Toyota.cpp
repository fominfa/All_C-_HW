class Toyota : public Car {
public:
    Toyota() : Car(170, "Toyota", 800) {}
    void ride_10_minutes() override {
        std::cout << "Toyota drove " << std::min(reserve, 15)
            << " kilometrs\n";
        reserve = std::max(reserve - 15, 0);
    }
    void refuel() override {
        std::cout << "Toyota has been refueled\n";
        reserve = 800;
    }
    void break_Toyota() const { std::cout << "Toyota never breaks down\n"; }
};
