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

void RPN::processToken(const std::string& token) {
    if (isValidNumber(token)) {
        double number = token[0] - '0';
        _stack.push(number);
    }
    else if (isOperator(token)) {
        if (_stack.size() < 2) {
            throw std::runtime_error("Error: insufficient operands");
        }
        double b = _stack.top(); _stack.pop();
        double a = _stack.top(); _stack.pop();
        double result = performOperation(a, b, token);
        _stack.push(result);
    }
    else {
        throw std::runtime_error("Error: invalid token '" + token + "'");
    }
}

double RPN::calculate(const std::string& expression) {
    while (!_stack.empty()) {
        _stack.pop();
    }
    std::istringstream iss(expression);
    std::string token;
    while (iss >> token) {
        processToken(token);
    }
    if (_stack.empty()) {
        throw std::runtime_error("Error: empty expression");
    }
    if (_stack.size() != 1) {
        throw std::runtime_error("Error: invalid expression - too many operands");
    }
    return _stack.top();
}
