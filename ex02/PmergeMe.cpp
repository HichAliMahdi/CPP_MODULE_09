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

