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
        std::vector<int> _primary;
        std::deque<int> _secondary;
    
        // Core sorting utilities
        static std::vector<int> calculateSequence(int limit);
    
        // Primary container algorithms
        static void mergeInsertAlgorithm(std::vector<int>& data);
        static void binaryInsertionProcess(std::vector<int>& data, int start, int end);
        static void combineSegments(std::vector<int>& data, int left, int center, int right);
    
        // Secondary container algorithms
        static void mergeInsertAlgorithm(std::deque<int>& data);
        static void binaryInsertionProcess(std::deque<int>& data, int start, int end);
        static void combineSegments(std::deque<int>& data, int left, int center, int right);
    
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
        void executePrimarySort();
        void executeSecondarySort();
        void showInitialState() const;
        void showFinalState() const;
};

#endif