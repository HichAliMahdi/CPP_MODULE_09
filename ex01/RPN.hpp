#ifndef RPN_HPP
# define RPN_HPP

#include <iostream>
#include <string>
#include <stack>
#include <sstream>
#include <stdexcept>

class RPN {
    private:
        std::stack<double> _stack;
        bool isOperator(const std::string& token) const;
        bool isValidNumber(const std::string& token) const;
        double performOperation(double a, double b, const std::string& op) const;
        void processToken(const std::string& token);
    public:
        RPN();
        ~RPN();
        RPN(const RPN& other);
        RPN& operator=(const RPN& other);

        double calculate(const std::string& expression);
};

#endif