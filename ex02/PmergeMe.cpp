#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& src) : _primary(src._primary), _secondary(src._secondary) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& src) {
    if (this != &src) {
        _primary = src._primary;
        _secondary = src._secondary;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}

bool PmergeMe::validateInput(const std::string& token) {
    if (token.empty()) return false;
    
    for (size_t idx = 0; idx < token.length(); ++idx) {
        if (!std::isdigit(token[idx])) return false;
    }
    return true;
}

int PmergeMe::convertToInteger(const std::string& token) {
    std::istringstream stream(token);
    int value;
    stream >> value;
    return value;
}

void PmergeMe::processArguments(int count, char** args) {
    if (count < 2) {
        throw std::runtime_error("Error: No arguments provided");
    }
    
    for (int idx = 1; idx < count; ++idx) {
        std::string current = args[idx];
        
        if (!validateInput(current)) {
            throw std::runtime_error("Error: Invalid argument: " + current);
        }
        
        int value = convertToInteger(current);
        if (value < 0) {
            throw std::runtime_error("Error: Negative number not allowed: " + current);
        }
        
        _primary.push_back(value);
        _secondary.push_back(value);
    }
    
    if (_primary.empty()) {
        throw std::runtime_error("Error: No valid numbers provided");
    }
}

