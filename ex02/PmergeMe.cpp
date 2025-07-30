#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& src) : _vectorData(src._vectorData), _dequeData(src._dequeData) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& src) {
    if (this != &src) {
        _vectorData = src._vectorData;
        _dequeData = src._dequeData;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}

std::vector<int> PmergeMe::createMainChain(const std::vector<std::pair<int, int>>& pairs) {
    std::vector<int> mainChain;
    if (pairs.empty()) return mainChain;
    
    mainChain.push_back(pairs[0].first);
    for (size_t i = 0; i < pairs.size(); ++i) {
        mainChain.push_back(pairs[i].second);
    }
    
    return mainChain;
}

bool PmergeMe::validateInput(const std::string &token)
{
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
        throw std::runtime_error("Error");
    }
    
    for (int idx = 1; idx < count; ++idx) {
        std::string current = args[idx];
        
        if (!validateInput(current)) {
            throw std::runtime_error("Error");
        }
        
        int value = convertToInteger(current);
        if (value < 0) {
            throw std::runtime_error("Error");
        }
        
        _vectorData.push_back(value);
        _dequeData.push_back(value);
    }
    
    if (_vectorData.empty()) {
        throw std::runtime_error("Error");
    }
}

double PmergeMe::getTimestamp() {
    struct timeval timeInfo;
    gettimeofday(&timeInfo, NULL);
    return timeInfo.tv_sec * 1000000.0 + timeInfo.tv_usec;
}

// Generate Jacobsthal numbers: J(n) = J(n-1) + 2*J(n-2), with J(0)=0, J(1)=1
size_t PmergeMe::getJacobsthalNumber(size_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    size_t j0 = 0, j1 = 1;
    for (size_t i = 2; i <= n; ++i) {
        size_t temp = j1 + 2 * j0;
        j0 = j1;
        j1 = temp;
    }
    return j1;
}

std::vector<size_t> PmergeMe::generateJacobsthalSequence(size_t n) {
    std::vector<size_t> sequence;
    if (n == 0) return sequence;
    
    size_t i = 1;
    while (true) {
        size_t jacob = getJacobsthalNumber(i);
        if (jacob >= n) break;
        sequence.push_back(jacob);
        ++i;
    }
    return sequence;
}

// Ford-Johnson algorithm for vector
void PmergeMe::fordJohnsonSort(std::vector<int>& data) {
    size_t n = data.size();
    if (n <= 1) return;
    
    std::vector<std::pair<int, int>> pairs;
    bool hasStraggler = (n % 2 == 1);
    int straggler = hasStraggler ? data[n - 1] : 0;
    
    for (size_t i = 0; i < n - (hasStraggler ? 1 : 0); i += 2) {
        int a = data[i];
        int b = data[i + 1];
        if (a > b) std::swap(a, b);
        pairs.push_back(std::make_pair(a, b));
    
    if (pairs.size() > 1) {
        std::vector<int> largerElements;
        for (size_t i = 0; i < pairs.size(); ++i) {
            largerElements.push_back(pairs[i].second);
        }
        
        fordJohnsonSort(largerElements);
        
        std::vector<std::pair<int, int>> sortedPairs;
        for (size_t i = 0; i < largerElements.size(); ++i) {
            for (size_t j = 0; j < pairs.size(); ++j) {
                if (pairs[j].second == largerElements[i]) {
                    sortedPairs.push_back(pairs[j]);
                    break;
                }
            }
        }
        pairs = sortedPairs;
    }
    
    std::vector<int> mainChain = createMainChain(pairs);
    
    std::vector<int> pending;
    for (size_t i = 0; i < pairs.size(); ++i) {
        pending.push_back(pairs[i].first);
    }
    if (hasStraggler) {
        pending.push_back(straggler);
    }
    
    insertPendingElements(mainChain, pending);
    
    data = mainChain;
}
