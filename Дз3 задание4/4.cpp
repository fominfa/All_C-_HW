#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>

class IPAddress {
    uint32_t value;

    static uint32_t parse_ip(const std::string& ip_str) {
        std::istringstream iss(ip_str);
        std::string token;
        uint32_t result = 0;

        int count = 0;
        while (std::getline(iss, token, '.')) {
            if (count >= 4) throw std::invalid_argument("Too many octets");
            int octet = std::stoi(token);
            if (octet < 0 || octet > 255)
                throw std::invalid_argument("Invalid octet value");
            result = (result << 8) | static_cast<uint32_t>(octet);
            ++count;
        }

        if (count != 4)
            throw std::invalid_argument("Wrong number of octets");

        return result;
    }

    static std::string to_string(uint32_t val) {
        std::ostringstream oss;
        oss << ((val >> 24) & 0xFF) << '.'
            << ((val >> 16) & 0xFF) << '.'
            << ((val >> 8) & 0xFF) << '.'
            << (val & 0xFF);
        return oss.str();
    }

public:
    IPAddress() : value(0) {}
    IPAddress(const std::string& ip_str) : value(parse_ip(ip_str)) {}
    explicit IPAddress(uint32_t val) : value(val) {}

    friend std::istream& operator>>(std::istream& is, IPAddress& ip) {
        std::string input;
        if (!(is >> input)) return is;
        ip.value = parse_ip(input);
        return is;
    }

    friend std::ostream& operator<<(std::ostream& os, const IPAddress& ip) {
        os << to_string(ip.value);
        return os;
    }

    bool operator<(const IPAddress& other) const {
        return value < other.value;
    }
    bool operator>(const IPAddress& other) const {
        return value > other.value;
    }
    bool operator<=(const IPAddress& other) const {
        return value <= other.value;
    }
    bool operator>=(const IPAddress& other) const {
        return value >= other.value;
    }

    uint32_t to_uint() const { return value; }
};