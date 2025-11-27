#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <map>
#include <memory>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <stdexcept>

class Operation {
public:
    virtual ~Operation() = default;
    virtual void execute(std::stack<double>& stack) const = 0;
    virtual int getArity() const = 0;
};

class UnaryOperation : public Operation {
public:
    int getArity() const override { return 1; }

protected:
    virtual double calculate(double a) const = 0;

    void execute(std::stack<double>& stack) const override {
        if (stack.empty()) {
            throw std::runtime_error("Not enough operands for unary operation");
        }
        double a = stack.top(); stack.pop();
        stack.push(calculate(a));
    }
};

class BinaryOperation : public Operation {
public:
    int getArity() const override { return 2; }

protected:
    virtual double calculate(double a, double b) const = 0;

    void execute(std::stack<double>& stack) const override {
        if (stack.size() < 2) {
            throw std::runtime_error("Not enough operands for binary operation");
        }
        double b = stack.top(); stack.pop();
        double a = stack.top(); stack.pop();
        stack.push(calculate(a, b));
    }
};

class TernaryOperation : public Operation {
public:
    int getArity() const override { return 3; }

protected:
    virtual double calculate(double a, double b, double c) const = 0;

    void execute(std::stack<double>& stack) const override {
        if (stack.size() < 3) {
            throw std::runtime_error("Not enough operands for ternary operation");
        }
        double c = stack.top(); stack.pop();
        double b = stack.top(); stack.pop();
        double a = stack.top(); stack.pop();
        stack.push(calculate(a, b, c));
    }
};

class AddOperation : public BinaryOperation {
protected:
    double calculate(double a, double b) const override {
        return a + b;
    }
};

class SubtractOperation : public BinaryOperation {
protected:
    double calculate(double a, double b) const override {
        return a - b;
    }
};

class MultiplyOperation : public BinaryOperation {
protected:
    double calculate(double a, double b) const override {
        return a * b;
    }
};

class DivideOperation : public BinaryOperation {
protected:
    double calculate(double a, double b) const override {
        if (b == 0) {
            throw std::runtime_error("Division by zero");
        }
        return a / b;
    }
};

class SinOperation : public UnaryOperation {
protected:
    double calculate(double a) const override {
        return std::sin(a);
    }
};

class CosOperation : public UnaryOperation {
protected:
    double calculate(double a) const override {
        return std::cos(a);
    }
};

class TanOperation : public UnaryOperation {
protected:
    double calculate(double a) const override {
        return std::tan(a);
    }
};

class CtgOperation : public UnaryOperation {
protected:
    double calculate(double a) const override {
        double tan_val = std::tan(a);
        if (tan_val == 0) {
            throw std::runtime_error("Cotangent undefined");
        }
        return 1.0 / tan_val;
    }
};

class ExpOperation : public UnaryOperation {
protected:
    double calculate(double a) const override {
        return std::exp(a);
    }
};

class LogOperation : public UnaryOperation {
protected:
    double calculate(double a) const override {
        if (a <= 0) {
            throw std::runtime_error("Logarithm of non-positive number");
        }
        return std::log(a);
    }
};

class SqrtOperation : public UnaryOperation {
protected:
    double calculate(double a) const override {
        if (a < 0) {
            throw std::runtime_error("Square root of negative number");
        }
        return std::sqrt(a);
    }
};

class Atan2Operation : public BinaryOperation {
protected:
    double calculate(double a, double b) const override {
        return std::atan2(a, b);
    }
};

class PowOperation : public BinaryOperation {
protected:
    double calculate(double a, double b) const override {
        return std::pow(a, b);
    }
};

class MedianOperation : public TernaryOperation {
protected:
    double calculate(double a, double b, double c) const override {
        std::vector<double> values = { a, b, c };
        std::sort(values.begin(), values.end());
        return values[1];
    }
};

class RPNCalculator {
private:
    std::stack<double> stack;
    std::map<std::string, std::shared_ptr<Operation>> operations;
    bool isNumber(const std::string& str) {
        try {
            std::size_t pos;
            std::stod(str, &pos);
            return pos == str.length();
        }
        catch (...) {
            return false;
        }
    }
public:
    RPNCalculator() {
        operations["+"] = std::make_shared<AddOperation>();
        operations["-"] = std::make_shared<SubtractOperation>();
        operations["*"] = std::make_shared<MultiplyOperation>();
        operations["/"] = std::make_shared<DivideOperation>();
        operations["sin"] = std::make_shared<SinOperation>();
        operations["cos"] = std::make_shared<CosOperation>();
        operations["tg"] = std::make_shared<TanOperation>();
        operations["ctg"] = std::make_shared<CtgOperation>();
        operations["exp"] = std::make_shared<ExpOperation>();
        operations["log"] = std::make_shared<LogOperation>();
        operations["sqrt"] = std::make_shared<SqrtOperation>();
        operations["atan2"] = std::make_shared<Atan2Operation>();
        operations["pow"] = std::make_shared<PowOperation>();
        operations["median"] = std::make_shared<MedianOperation>();
    }
    double evaluate(const std::string& expression) {
        std::istringstream iss(expression);
        std::string token;
        while (iss >> token) {
            if (isNumber(token)) {
                stack.push(std::stod(token));
            }
            else if (operations.find(token) != operations.end()) {
                operations[token]->execute(stack);
            }
            else {
                throw std::runtime_error("Unknown operation: " + token);
            }
        }
        if (stack.size() != 1) {
            throw std::runtime_error("Invalid expression: stack has " +
                std::to_string(stack.size()) + " elements instead of 1");
        }
        double result = stack.top();
        stack.pop();
        return result;
    }
    void clear() {
        while (!stack.empty()) {
            stack.pop();
        }
    }
};

int main() {
    RPNCalculator calculator;
    std::string st;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, st);
        if (st == "quit" || st == "exit") {
            break;
        }
        if (st.empty()) {
            continue;
        }
        try {
            double result = calculator.evaluate(st);
            std::cout << "Result: " << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
        calculator.clear();
    }
    return 0;
}