#include <iostream>
#include <numeric>
#include <ostream>
#include <istream>

class Rational {
private:
    int numerator;
    int denominator;

    void reduce() {
        if (denominator == 0) throw std::invalid_argument("Denominator cannot be zero");
        int gcd = std::gcd(numerator, denominator);
        numerator /= gcd;
        denominator /= gcd;
        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }
    }

public:
    Rational(int num = 0, int den = 1) : numerator(num), denominator(den) {
        reduce();
    }

    operator double() const {
        return static_cast<double>(numerator) / denominator;
    }

    Rational& operator+=(const Rational& rhs) {
        numerator = numerator * rhs.denominator + rhs.numerator * denominator;
        denominator *= rhs.denominator;
        reduce();
        return *this;
    }

    Rational& operator-=(const Rational& rhs) {
        numerator = numerator * rhs.denominator - rhs.numerator * denominator;
        denominator *= rhs.denominator;
        reduce();
        return *this;
    }

    Rational& operator*=(const Rational& rhs) {
        numerator *= rhs.numerator;
        denominator *= rhs.denominator;
        reduce();
        return *this;
    }

    Rational& operator/=(const Rational& rhs) {
        if (rhs.numerator == 0) throw std::invalid_argument("Division by zero");
        numerator *= rhs.denominator;
        denominator *= rhs.numerator;
        reduce();
        return *this;
    }

    Rational& operator++() {
        *this += 1;
        return *this;
    }

    Rational& operator--() {
        *this -= 1;
        return *this;
    }

    Rational operator++(int) {
        Rational temp = *this;
        ++(*this);
        return temp;
    }

    Rational operator--(int) {
        Rational temp = *this;
        --(*this);
        return temp;
    }

    friend Rational operator+(Rational lhs, const Rational& rhs) {
        lhs += rhs;
        return lhs;
    }

    friend Rational operator-(Rational lhs, const Rational& rhs) {
        lhs -= rhs;
        return lhs;
    }

    friend Rational operator*(Rational lhs, const Rational& rhs) {
        lhs *= rhs;
        return lhs;
    }

    friend Rational operator/(Rational lhs, const Rational& rhs) {
        lhs /= rhs;
        return lhs;
    }

    friend bool operator==(const Rational& lhs, const Rational& rhs) {
        return lhs.numerator == rhs.numerator && lhs.denominator == rhs.denominator;
    }

    friend bool operator!=(const Rational& lhs, const Rational& rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<(const Rational& lhs, const Rational& rhs) {
        return lhs.numerator * rhs.denominator < rhs.numerator * lhs.denominator;
    }

    friend bool operator<=(const Rational& lhs, const Rational& rhs) {
        return !(rhs < lhs);
    }

    friend bool operator>(const Rational& lhs, const Rational& rhs) {
        return rhs < lhs;
    }

    friend bool operator>=(const Rational& lhs, const Rational& rhs) {
        return !(lhs < rhs);
    }

    friend std::ostream& operator<<(std::ostream& os, const Rational& r) {
        os << r.numerator << "/" << r.denominator;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Rational& r) {
        int num, den;
        char sep;
        is >> num >> sep >> den;
        if (sep != '/') {
            is.setstate(std::ios::failbit);
            return is;
        }
        r = Rational(num, den);
        return is;
    }
};