#include "BitcoinExchange.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Error: not enough or too many arguments" << std::endl;
        return 1;
    }
    BitcoinExchange ex;
    if (!ex.loadDatabase()) {
        return 1;
    }
    ex.processInputFile(argv[1]);
    return 0;
}
