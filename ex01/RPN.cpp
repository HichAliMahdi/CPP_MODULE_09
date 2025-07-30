#include "RPN.hpp"

RPN::RPN(){}
RPN::RPN(const RPN& other) : _stack(other._stack){}
RPN& RPN::operator=(const RPN& other) {
    if (this != &other) {
        _stack = other._stack;
    }
    return *this;
}
RPN::~RPN(){}

bool RPN::isOperator(const std::string& token) const {
    return (token == "+" || token == "-" || token == "*" || token == "/");
}

bool RPN::isValidNumber(const std::string& token) const {
    if (token.length() != 1) {
        return false;
    }
    return (token[0] >= '0' && token[0] <= '9');
}

double RPN::performOperation(double a, double b, const std::string& op) const {
    if (op == "+") {
        return a + b;
    }
    else if (op == "-") {
        return a - b;
    }
    else if (op == "*") {
        return a * b;
    }
    else if (op == "/") {
        if (b == 0) {
            throw std::runtime_error("Error: division by zero");
        }
        return a / b;
    }
    else {
        throw std::runtime_error("Error: unknown operator");
    }
}

