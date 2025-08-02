#include "PmergeMe.hpp"
#include <iostream>
#include <iomanip>

int main(int argc, char* argv[]){
    try{
        PmergeMe processor;
        processor.processArguments(argc, argv);
        processor.showInitialState();

        PmergeMe vectorProcessor = processor;
        double startTime = PmergeMe::getTimestamp();
        vectorProcessor.executeVectorSort();
        double vectorTime = PmergeMe::getTimestamp() - startTime;

        PmergeMe dequeProcessor = processor;
        startTime = PmergeMe::getTimestamp();
        dequeProcessor.executeDequeSort();
        double dequeTime = PmergeMe::getTimestamp() - startTime;

        vectorProcessor.showFinalState();

        size_t elementCount = vectorProcessor.getSize();
        std::cout << std::fixed << std::setprecision(5);
        std::cout << "Time to process a range of " << elementCount 
                  << " elements with std::vector : " << vectorTime << " µs" << std::endl;
        std::cout << "Time to process a range of " << elementCount 
                  << " elements with std::deque : " << dequeTime << " µs" << std::endl;
        
    } catch (const std::exception& error) {
        std::cerr << error.what() << std::endl;
        return 1;
    }
    
    return 0;
}