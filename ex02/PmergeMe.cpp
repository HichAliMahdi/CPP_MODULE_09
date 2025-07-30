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
    
    // Step 1: Pair elements and sort pairs
    std::vector<std::pair<int, int>> pairs;
    bool hasStraggler = (n % 2 == 1);
    int straggler = hasStraggler ? data[n - 1] : 0;
    
    for (size_t i = 0; i < n - (hasStraggler ? 1 : 0); i += 2) {
        int a = data[i];
        int b = data[i + 1];
        if (a > b) std::swap(a, b);
        pairs.push_back(std::make_pair(a, b)); // smaller, larger
    }
    
    // Step 2: Recursively sort pairs by their larger elements
    if (pairs.size() > 1) {
        std::vector<int> largerElements;
        for (size_t i = 0; i < pairs.size(); ++i) {
            largerElements.push_back(pairs[i].second);
        }
        
        fordJohnsonSort(largerElements);
        
        // Reorder pairs based on sorted larger elements
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
    
    // Step 3: Create main chain
    std::vector<int> mainChain = createMainChainVector(pairs);
    
    // Step 4: Create pending elements
    std::vector<int> pending;
    for (size_t i = 0; i < pairs.size(); ++i) {
        pending.push_back(pairs[i].first);
    }
    if (hasStraggler) {
        pending.push_back(straggler);
    }
    
    // Step 5: Insert pending elements using Jacobsthal sequence
    insertPendingElementsVector(mainChain, pending);
    
    data = mainChain;
}

std::vector<int> PmergeMe::createMainChainVector(const std::vector<std::pair<int, int>>& pairs) {
    std::vector<int> mainChain;
    if (pairs.empty()) return mainChain;
    
    // Add first smaller element and all larger elements
    mainChain.push_back(pairs[0].first);
    for (size_t i = 0; i < pairs.size(); ++i) {
        mainChain.push_back(pairs[i].second);
    }
    
    return mainChain;
}

void PmergeMe::insertPendingElementsVector(std::vector<int>& mainChain, const std::vector<int>& pending) {
    if (pending.empty()) return;
    
    std::vector<size_t> jacobSeq = generateJacobsthalSequence(pending.size());
    std::vector<bool> inserted(pending.size(), false);
    
    // Insert elements according to Jacobsthal sequence
    for (size_t i = 0; i < jacobSeq.size(); ++i) {
        size_t jacobIdx = jacobSeq[i] - 1; // Convert to 0-based index
        if (jacobIdx < pending.size() && !inserted[jacobIdx]) {
            size_t pos = binarySearchInsertPos(mainChain, pending[jacobIdx], 0, mainChain.size());
            mainChain.insert(mainChain.begin() + pos, pending[jacobIdx]);
            inserted[jacobIdx] = true;
        }
    }
    
    // Insert remaining elements
    for (size_t i = 0; i < pending.size(); ++i) {
        if (!inserted[i]) {
            size_t pos = binarySearchInsertPos(mainChain, pending[i], 0, mainChain.size());
            mainChain.insert(mainChain.begin() + pos, pending[i]);
        }
    }
}

size_t PmergeMe::binarySearchInsertPos(const std::vector<int>& arr, int value, size_t left, size_t right) {
    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (arr[mid] < value) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

// Ford-Johnson algorithm for deque (same logic, different container)
void PmergeMe::fordJohnsonSort(std::deque<int>& data) {
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
    }
    
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
    
    std::deque<int> mainChain = createMainChainDeque(pairs);
    
    std::vector<int> pending;
    for (size_t i = 0; i < pairs.size(); ++i) {
        pending.push_back(pairs[i].first);
    }
    if (hasStraggler) {
        pending.push_back(straggler);
    }
    
    insertPendingElementsDeque(mainChain, pending);
    
    data = mainChain;
}

std::deque<int> PmergeMe::createMainChainDeque(const std::vector<std::pair<int, int>>& pairs) {
    std::deque<int> mainChain;
    if (pairs.empty()) return mainChain;
    
    mainChain.push_back(pairs[0].first);
    for (size_t i = 0; i < pairs.size(); ++i) {
        mainChain.push_back(pairs[i].second);
    }
    
    return mainChain;
}

void PmergeMe::insertPendingElementsDeque(std::deque<int>& mainChain, const std::vector<int>& pending) {
    if (pending.empty()) return;
    
    std::vector<size_t> jacobSeq = generateJacobsthalSequence(pending.size());
    std::vector<bool> inserted(pending.size(), false);
    
    for (size_t i = 0; i < jacobSeq.size(); ++i) {
        size_t jacobIdx = jacobSeq[i] - 1;
        if (jacobIdx < pending.size() && !inserted[jacobIdx]) {
            size_t pos = binarySearchInsertPos(mainChain, pending[jacobIdx], 0, mainChain.size());
            mainChain.insert(mainChain.begin() + pos, pending[jacobIdx]);
            inserted[jacobIdx] = true;
        }
    }
    for (size_t i = 0; i < pending.size(); ++i) {
        if (!inserted[i]) {
            size_t pos = binarySearchInsertPos(mainChain, pending[i], 0, mainChain.size());
            mainChain.insert(mainChain.begin() + pos, pending[i]);
        }
    }
}

size_t PmergeMe::binarySearchInsertPos(const std::deque<int>& arr, int value, size_t left, size_t right) {
    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (arr[mid] < value) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

void PmergeMe::executeVectorSort() {
    fordJohnsonSort(_vectorData);
}

void PmergeMe::executeDequeSort() {
    fordJohnsonSort(_dequeData);
}

void PmergeMe::showInitialState() const {
    std::cout << "Before: ";
    for (size_t idx = 0; idx < _vectorData.size(); ++idx) {
        std::cout << _vectorData[idx];
        if (idx < _vectorData.size() - 1) std::cout << " ";
    }
    std::cout << std::endl;
}

void PmergeMe::showFinalState() const {
    std::cout << "After: ";
    for (size_t idx = 0; idx < _vectorData.size(); ++idx) {
        std::cout << _vectorData[idx];
        if (idx < _vectorData.size() - 1) std::cout << " ";
    }
    std::cout << std::endl;
}
