#include "PmergeMe.hpp"
#include <iostream>
#include <iomanip>

int main(int argc, char* argv[]){
    try{
        PmergeMe processor;

        processor.processArguments(argc, argv);

        processor.showInitialState();

        PmergeMe firstProcessor = processor;
        double beginTime = PmergeMe::getTimestamp();
        firstProcessor.executePrimarySort();
        double primaryDuration = PmergeMe::getTimestamp() - beginTime;

        PmergeMe secondProcessor = processor;
        beginTime = PmergeMe::getTimestamp();
        secondProcessor.executeSecondarySort();
        double secondaryDuration = PmergeMe::getTimestamp() - beginTime;

        firstProcessor.showFinalState();

        int elementCount = argc - 1;
        std::cout << std::fixed << std::setprecision(5);
        std::cout << "Time to process a range of " << elementCount 
                  << " elements with std::vector : " << primaryDuration << " us" << std::endl;
        std::cout << "Time to process a range of " << elementCount 
                  << " elements with std::deque : " << secondaryDuration << " us" << std::endl;
        
    } catch (const std::exception& error) {
        std::cerr << error.what() << std::endl;
        return 1;
    }
    
    return 0;
}
