#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <sys/time.h>
#include <algorithm>

class PmergeMe {
    private:
        std::vector<int> _vectorData;
        std::deque<int> _dequeData;
    
        // Jacobsthal number generation
        static std::vector<size_t> generateJacobsthalSequence(size_t n);
        static size_t getJacobsthalNumber(size_t n);
    
        // Ford-Johnson algorithm for vector
        static void fordJohnsonSort(std::vector<int>& data);
        static std::vector<int> createMainChainVector(const std::vector<std::pair<int, int>>& pairs);
        static void insertPendingElementsVector(std::vector<int>& mainChain, 
                                               const std::vector<int>& pending);
        static size_t binarySearchInsertPos(const std::vector<int>& arr, int value, 
                                          size_t left, size_t right);
    
        // Ford-Johnson algorithm for deque
        static void fordJohnsonSort(std::deque<int>& data);
        static std::deque<int> createMainChainDeque(const std::vector<std::pair<int, int>>& pairs);
        static void insertPendingElementsDeque(std::deque<int>& mainChain, 
                                              const std::vector<int>& pending);
        static size_t binarySearchInsertPos(const std::deque<int>& arr, int value, 
                                          size_t left, size_t right);
    
        // Helper utilities
        static bool validateInput(const std::string& token);
        static int convertToInteger(const std::string& token);

    public:
        static double getTimestamp();
    
        PmergeMe();
        PmergeMe(const PmergeMe& src);
        PmergeMe& operator=(const PmergeMe& src);
        ~PmergeMe();
    
        void processArguments(int count, char** args);
        void executeVectorSort();
        void executeDequeSort();
        void showInitialState() const;
        void showFinalState() const;
        size_t getSize() const { return _vectorData.size(); }
};

#endif