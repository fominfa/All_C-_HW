#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <cstring>
#include <memory>

class BCD {
private:
    std::vector<uint8_t> digits_;
    int precision_;
    bool negative_;
    
    void remove_leading_zeros();
    void remove_trailing_zeros();
    void normalize();
    void align_precision(BCD& other);
    size_t integer_digits_count() const;
    
    BCD add_absolute(const BCD& other) const;
    BCD subtract_absolute(const BCD& other) const;
    BCD multiply_absolute(const BCD& other) const;
    
public:
    BCD() : precision_(0), negative_(false) {}
    
    BCD(int value) : negative_(value < 0) {
        precision_ = 0;
        value = std::abs(value);
        
        if (value == 0) {
            digits_.push_back(0);
            return;
        }
        
        while (value > 0) {
            digits_.insert(digits_.begin(), value % 10);
            value /= 10;
        }
    }
    
    BCD(const std::string& str);
    
    BCD(const BCD& other) = default;
    BCD& operator=(const BCD& other) = default;
    BCD(BCD&& other) = default;
    BCD& operator=(BCD&& other) = default;
    
    BCD operator+() const { return *this; }
    BCD operator-() const;
    
    BCD operator+(const BCD& other) const;
    BCD operator-(const BCD& other) const;
    BCD operator*(const BCD& other) const;
    
    BCD& operator+=(const BCD& other);
    BCD& operator-=(const BCD& other);
    BCD& operator*=(const BCD& other);
    
    bool operator==(const BCD& other) const;
    bool operator!=(const BCD& other) const { return !(*this == other); }
    bool operator<(const BCD& other) const;
    bool operator>(const BCD& other) const { return other < *this; }
    bool operator<=(const BCD& other) const { return !(other < *this); }
    bool operator>=(const BCD& other) const { return !(*this < other); }
    
    int get_precision() const { return precision_; }
    bool is_negative() const { return negative_; }
    
    std::string to_string() const;
    int to_int() const;
    
    friend BCD reciprocal(int N, int precision);
    friend BCD calculate_e(int precision);
    
private:
    static BCD factorial(int n);
    static BCD power(const BCD& base, int exponent);
};

BCD::BCD(const std::string& str) : negative_(false), precision_(0) {
    if (str.empty()) {
        digits_.push_back(0);
        return;
    }
    
    size_t start = 0;
    if (str[0] == '-') {
        negative_ = true;
        start = 1;
    } else if (str[0] == '+') {
        start = 1;
    }
    
    size_t dot_pos = str.find('.');
    std::string integer_part, fractional_part;
    
    if (dot_pos == std::string::npos) {
        integer_part = str.substr(start);
        fractional_part = "";
    } else {
        integer_part = str.substr(start, dot_pos - start);
        fractional_part = str.substr(dot_pos + 1);
    }
    
    for (char c : integer_part) {
        if (c >= '0' && c <= '9') {
            digits_.push_back(c - '0');
        } else {
            throw std::invalid_argument("Invalid character in BCD string");
        }
    }
    
    for (char c : fractional_part) {
        if (c >= '0' && c <= '9') {
            digits_.push_back(c - '0');
            precision_++;
        } else {
            throw std::invalid_argument("Invalid character in BCD string");
        }
    }
    
    remove_leading_zeros();
    
    if (integer_digits_count() > 10) {
        throw std::overflow_error("Integer part exceeds 10 digits");
    }
    
    if (digits_.empty() || (digits_.size() == 1 && digits_[0] == 0)) {
        negative_ = false;
    }
}

void BCD::remove_leading_zeros() {
    size_t integer_digits = integer_digits_count();
    size_t leading_zeros = 0;
    
    for (size_t i = 0; i < integer_digits - 1; ++i) {
        if (digits_[i] == 0) {
            leading_zeros++;
        } else {
            break;
        }
    }
    
    if (leading_zeros > 0) {
        digits_.erase(digits_.begin(), digits_.begin() + leading_zeros);
    }
}

void BCD::remove_trailing_zeros() {
    while (precision_ > 0 && !digits_.empty() && digits_.back() == 0) {
        digits_.pop_back();
        precision_--;
    }
}

void BCD::normalize() {
    remove_leading_zeros();
    remove_trailing_zeros();
    
    if (digits_.empty()) {
        digits_.push_back(0);
        precision_ = 0;
        negative_ = false;
    }
}

size_t BCD::integer_digits_count() const {
    return digits_.size() - precision_;
}

void BCD::align_precision(BCD& other) {
    int max_precision = std::max(precision_, other.precision_);
    
    while (precision_ < max_precision) {
        digits_.push_back(0);
        precision_++;
    }
    
    while (other.precision_ < max_precision) {
        other.digits_.push_back(0);
        other.precision_++;
    }
    
    size_t this_int_digits = integer_digits_count();
    size_t other_int_digits = other.integer_digits_count();
    size_t max_int_digits = std::max(this_int_digits, other_int_digits);
    
    while (this_int_digits < max_int_digits) {
        digits_.insert(digits_.begin(), 0);
        this_int_digits++;
    }
    
    while (other_int_digits < max_int_digits) {
        other.digits_.insert(other.digits_.begin(), 0);
        other_int_digits++;
    }
}

BCD BCD::operator-() const {
    BCD result = *this;
    if (!(digits_.size() == 1 && digits_[0] == 0)) {
        result.negative_ = !result.negative_;
    }
    return result;
}

BCD BCD::add_absolute(const BCD& other) const {
    BCD a = *this;
    BCD b = other;
    a.align_precision(b);
    
    BCD result;
    result.precision_ = a.precision_;
    result.negative_ = false;
    
    int carry = 0;
    for (int i = a.digits_.size() - 1; i >= 0; --i) {
        int sum = a.digits_[i] + b.digits_[i] + carry;
        result.digits_.insert(result.digits_.begin(), sum % 10);
        carry = sum / 10;
    }
    
    if (carry > 0) {
        result.digits_.insert(result.digits_.begin(), carry);
    }
    
    if (result.precision_ > 0) {
        result.precision_ = std::max(0, result.precision_ - 1);
        if (result.digits_.size() > result.integer_digits_count() + result.precision_) {
            result.digits_.pop_back();
        }
    }
    
    result.normalize();
    return result;
}

BCD BCD::subtract_absolute(const BCD& other) const {
    BCD a = *this;
    BCD b = other;
    a.align_precision(b);
    
    BCD result;
    result.precision_ = a.precision_;
    result.negative_ = false;
    
    int borrow = 0;
    for (int i = a.digits_.size() - 1; i >= 0; --i) {
        int diff = a.digits_[i] - b.digits_[i] - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.digits_.insert(result.digits_.begin(), diff);
    }
    
    if (result.precision_ > 0) {
        result.precision_ = std::max(0, result.precision_ - 1);
        if (result.digits_.size() > result.integer_digits_count() + result.precision_) {
            result.digits_.pop_back();
        }
    }
    
    result.normalize();
    return result;
}

BCD BCD::multiply_absolute(const BCD& other) const {
    BCD result;
    result.precision_ = precision_ + other.precision_;
    
    size_t total_digits = digits_.size() + other.digits_.size();
    result.digits_.resize(total_digits, 0);
    
    for (int i = digits_.size() - 1; i >= 0; --i) {
        int carry = 0;
        for (int j = other.digits_.size() - 1; j >= 0; --j) {
            int pos = i + j + 1;
            int product = digits_[i] * other.digits_[j] + result.digits_[pos] + carry;
            result.digits_[pos] = product % 10;
            carry = product / 10;
        }
        result.digits_[i] += carry;
    }
    
    BCD abs_a = *this;
    abs_a.negative_ = false;
    BCD abs_b = other;
    abs_b.negative_ = false;
    
    int ceil_a = std::ceil(abs_a.to_int());
    int ceil_b = std::ceil(abs_b.to_int());
    int max_ceil = std::max(ceil_a, ceil_b);
    
    int precision_loss = 1 + static_cast<int>(std::log10(max_ceil));
    result.precision_ = std::max(0, result.precision_ - precision_loss);
    
    size_t target_size = result.integer_digits_count() + result.precision_;
    if (result.digits_.size() > target_size) {
        result.digits_.resize(target_size);
    }
    
    result.normalize();
    return result;
}

BCD BCD::operator+(const BCD& other) const {
    if (negative_ == other.negative_) {
        BCD result = add_absolute(other);
        result.negative_ = negative_;
        return result;
    } else {
        if (!negative_) {
            if (*this >= -other) {
                return subtract_absolute(other);
            } else {
                BCD result = other.subtract_absolute(*this);
                result.negative_ = true;
                return result;
            }
        } else {
            return other + *this;
        }
    }
}

BCD BCD::operator-(const BCD& other) const {
    return *this + (-other);
}

BCD BCD::operator*(const BCD& other) const {
    BCD result = multiply_absolute(other);
    result.negative_ = negative_ != other.negative_;
    return result;
}

BCD& BCD::operator+=(const BCD& other) {
    *this = *this + other;
    return *this;
}

BCD& BCD::operator-=(const BCD& other) {
    *this = *this - other;
    return *this;
}

BCD& BCD::operator*=(const BCD& other) {
    *this = *this * other;
    return *this;
}

bool BCD::operator==(const BCD& other) const {
    BCD a = *this;
    BCD b = other;
    a.align_precision(b);
    a.normalize();
    b.normalize();
    
    return a.negative_ == b.negative_ && a.digits_ == b.digits_ && a.precision_ == b.precision_;
}

bool BCD::operator<(const BCD& other) const {
    if (negative_ != other.negative_) {
        return negative_;
    }
    
    if (negative_) {
        return (-other) < (-*this);
    }
    
    BCD a = *this;
    BCD b = other;
    a.align_precision(b);
    
    for (size_t i = 0; i < a.digits_.size(); ++i) {
        if (a.digits_[i] != b.digits_[i]) {
            return a.digits_[i] < b.digits_[i];
        }
    }
    
    return false;
}

std::string BCD::to_string() const {
    if (digits_.empty()) {
        return "0";
    }
    
    std::string result;
    if (negative_) {
        result += '-';
    }
    
    size_t int_digits = integer_digits_count();
    
    if (int_digits == 0) {
        result += '0';
    } else {
        for (size_t i = 0; i < int_digits; ++i) {
            result += static_cast<char>('0' + digits_[i]);
        }
    }
    
    if (precision_ > 0) {
        result += '.';
        for (size_t i = int_digits; i < digits_.size(); ++i) {
            result += static_cast<char>('0' + digits_[i]);
        }
    }
    
    return result;
}

int BCD::to_int() const {
    int result = 0;
    size_t int_digits = integer_digits_count();
    
    for (size_t i = 0; i < int_digits; ++i) {
        result = result * 10 + digits_[i];
    }
    
    return negative_ ? -result : result;
}

BCD BCD::factorial(int n) {
    if (n <= 1) return BCD(1);
    
    BCD result(1);
    for (int i = 2; i <= n; ++i) {
        result *= BCD(i);
    }
    return result;
}

BCD BCD::power(const BCD& base, int exponent) {
    if (exponent == 0) return BCD(1);
    
    BCD result(1);
    BCD current = base;
    int exp = std::abs(exponent);
    
    while (exp > 0) {
        if (exp % 2 == 1) {
            result *= current;
        }
        current *= current;
        exp /= 2;
    }
    
    if (exponent < 0) {
        throw std::runtime_error("Negative exponent not supported without division");
    }
    
    return result;
}

BCD reciprocal(int N, int precision) {
    if (N == 0) {
        throw std::invalid_argument("Division by zero");
    }
    
    BCD result("0");
    BCD remainder(1);
    BCD divisor(N);
    
    if (std::abs(N) == 1) {
        result = BCD(N > 0 ? 1 : -1);
        result.precision_ = 0;
    }
    
    for (int i = 0; i < precision + 10; ++i) {
        remainder *= BCD(10);
        
        int digit = 0;
        BCD temp_divisor = divisor;
        if (temp_divisor.is_negative()) {
            temp_divisor = -temp_divisor;
        }
        
        BCD temp_remainder = remainder;
        if (temp_remainder.is_negative()) {
            temp_remainder = -temp_remainder;
        }
        
        while (temp_remainder >= temp_divisor) {
            temp_remainder = temp_remainder - temp_divisor;
            digit++;
        }
        
        result.digits_.push_back(digit);
        result.precision_++;
        remainder = temp_remainder;
        
        if (remainder.to_int() == 0) {
            break;
        }
        
        if (remainder.is_negative()) {
            remainder = -remainder;
        }
    }
    
    if (N < 0) {
        result.negative_ = !result.negative_;
    }
    
    if (result.precision_ > precision) {
        result.digits_.resize(result.integer_digits_count() + precision);
        result.precision_ = precision;
    }
    
    result.normalize();
    return result;
}

BCD calculate_e(int precision) {
    BCD result("0");
    BCD term(1);
    
    int N = 1;
    while (true) {
        BCD fact = BCD::factorial(N + 1);
        BCD error_bound = reciprocal(fact.to_int(), precision + 2);
        error_bound *= BCD(3);
        
        BCD threshold("0.");
        for (int i = 0; i < precision - 1; ++i) {
            threshold.digits_.push_back(0);
        }
        threshold.digits_.push_back(1);
        threshold.precision_ = precision;
        
        if (error_bound < threshold) {
            break;
        }
        N++;
    }
    
    for (int n = 0; n <= N; ++n) {
        if (n > 0) {
            term = reciprocal(BCD::factorial(n).to_int(), precision + 5);
        }
        
        result += term;
        
        if (result.precision_ > precision + 5) {
            result.digits_.resize(result.integer_digits_count() + precision + 5);
            result.precision_ = precision + 5;
        }
    }
    
    if (result.precision_ > precision) {
        result.digits_.resize(result.integer_digits_count() + precision);
        result.precision_ = precision;
    }
    
    result.normalize();
    return result;
}