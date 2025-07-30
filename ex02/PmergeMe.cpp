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

double PmergeMe::getTimestamp() {
    struct timeval timeInfo;
    gettimeofday(&timeInfo, NULL);
    return timeInfo.tv_sec * 1000000.0 + timeInfo.tv_usec;
}

std::vector<int> PmergeMe::calculateSequence(int limit) {
    std::vector<int> sequence;
    if (limit <= 0) return sequence;
    
    sequence.push_back(1);
    if (limit == 1) return sequence;
    
    sequence.push_back(3);
    if (limit == 2) return sequence;
    
    for (int pos = 2; pos < limit; ++pos) {
        int nextVal = sequence[pos-1] + 2 * sequence[pos-2];
        if (nextVal > limit) break;
        sequence.push_back(nextVal);
    }
    
    return sequence;
}

void PmergeMe::binaryInsertionProcess(std::vector<int>& data, int start, int end) {
    for (int current = start + 1; current <= end; ++current) {
        int element = data[current];
        int position = current - 1;
        
        while (position >= start && data[position] > element) {
            data[position + 1] = data[position];
            --position;
        }
        data[position + 1] = element;
    }
}

void PmergeMe::combineSegments(std::vector<int>& data, int start, int center, int end) {
    std::vector<int> leftPart(data.begin() + start, data.begin() + center + 1);
    std::vector<int> rightPart(data.begin() + center + 1, data.begin() + end + 1);
    
    int leftIdx = 0, rightIdx = 0, mainIdx = start;
    
    while (leftIdx < static_cast<int>(leftPart.size()) && rightIdx < static_cast<int>(rightPart.size())) {
        if (leftPart[leftIdx] <= rightPart[rightIdx]) {
            data[mainIdx++] = leftPart[leftIdx++];
        } else {
            data[mainIdx++] = rightPart[rightIdx++];
        }
    }
    
    while (leftIdx < static_cast<int>(leftPart.size())) {
        data[mainIdx++] = leftPart[leftIdx++];
    }
    
    while (rightIdx < static_cast<int>(rightPart.size())) {
        data[mainIdx++] = rightPart[rightIdx++];
    }
}

void PmergeMe::mergeInsertAlgorithm(std::vector<int>& data) {
    int size = data.size();
    if (size <= 1) return;
    
    // Use insertion for small arrays
    if (size <= 20) {
        binaryInsertionProcess(data, 0, size - 1);
        return;
    }
    
    // Divide and conquer approach
    int midpoint = size / 2;
    
    std::vector<int> leftHalf(data.begin(), data.begin() + midpoint);
    std::vector<int> rightHalf(data.begin() + midpoint, data.end());
    
    mergeInsertAlgorithm(leftHalf);
    mergeInsertAlgorithm(rightHalf);
    
    // Combine sorted halves
    int leftPtr = 0, rightPtr = 0, dataPtr = 0;
    
    while (leftPtr < static_cast<int>(leftHalf.size()) && rightPtr < static_cast<int>(rightHalf.size())) {
        if (leftHalf[leftPtr] <= rightHalf[rightPtr]) {
            data[dataPtr++] = leftHalf[leftPtr++];
        } else {
            data[dataPtr++] = rightHalf[rightPtr++];
        }
    }
    
    while (leftPtr < static_cast<int>(leftHalf.size())) {
        data[dataPtr++] = leftHalf[leftPtr++];
    }
    
    while (rightPtr < static_cast<int>(rightHalf.size())) {
        data[dataPtr++] = rightHalf[rightPtr++];
    }
}

void PmergeMe::binaryInsertionProcess(std::deque<int>& data, int start, int end) {
    for (int current = start + 1; current <= end; ++current) {
        int element = data[current];
        int position = current - 1;
        
        while (position >= start && data[position] > element) {
            data[position + 1] = data[position];
            --position;
        }
        data[position + 1] = element;
    }
}

void PmergeMe::combineSegments(std::deque<int>& data, int start, int center, int end) {
    std::deque<int> leftPart(data.begin() + start, data.begin() + center + 1);
    std::deque<int> rightPart(data.begin() + center + 1, data.begin() + end + 1);
    
    int leftIdx = 0, rightIdx = 0, mainIdx = start;
    
    while (leftIdx < static_cast<int>(leftPart.size()) && rightIdx < static_cast<int>(rightPart.size())) {
        if (leftPart[leftIdx] <= rightPart[rightIdx]) {
            data[mainIdx++] = leftPart[leftIdx++];
        } else {
            data[mainIdx++] = rightPart[rightIdx++];
        }
    }
    
    while (leftIdx < static_cast<int>(leftPart.size())) {
        data[mainIdx++] = leftPart[leftIdx++];
    }
    
    while (rightIdx < static_cast<int>(rightPart.size())) {
        data[mainIdx++] = rightPart[rightIdx++];
    }
}

void PmergeMe::mergeInsertAlgorithm(std::deque<int>& data) {
    int size = data.size();
    if (size <= 1) return;
    
    // Use insertion for small arrays
    if (size <= 20) {
        binaryInsertionProcess(data, 0, size - 1);
        return;
    }
    
    // Divide and conquer approach
    int midpoint = size / 2;
    
    std::deque<int> leftHalf(data.begin(), data.begin() + midpoint);
    std::deque<int> rightHalf(data.begin() + midpoint, data.end());
    
    mergeInsertAlgorithm(leftHalf);
    mergeInsertAlgorithm(rightHalf);
    
    // Combine sorted halves
    int leftPtr = 0, rightPtr = 0, dataPtr = 0;
    
    while (leftPtr < static_cast<int>(leftHalf.size()) && rightPtr < static_cast<int>(rightHalf.size())) {
        if (leftHalf[leftPtr] <= rightHalf[rightPtr]) {
            data[dataPtr++] = leftHalf[leftPtr++];
        } else {
            data[dataPtr++] = rightHalf[rightPtr++];
        }
    }
    
    while (leftPtr < static_cast<int>(leftHalf.size())) {
        data[dataPtr++] = leftHalf[leftPtr++];
    }
    
    while (rightPtr < static_cast<int>(rightHalf.size())) {
        data[dataPtr++] = rightHalf[rightPtr++];
    }
}

void PmergeMe::executePrimarySort() {
    mergeInsertAlgorithm(_primary);
}

void PmergeMe::executeSecondarySort() {
    mergeInsertAlgorithm(_secondary);
}

void PmergeMe::showInitialState() const {
    std::cout << "Before: ";
    for (size_t idx = 0; idx < _primary.size(); ++idx) {
        std::cout << _primary[idx];
        if (idx < _primary.size() - 1) std::cout << " ";
    }
    std::cout << std::endl;
}

void PmergeMe::showFinalState() const {
    std::cout << "After: ";
    for (size_t idx = 0; idx < _primary.size(); ++idx) {
        std::cout << _primary[idx];
        if (idx < _primary.size() - 1) std::cout << " ";
    }
    std::cout << std::endl;
}
