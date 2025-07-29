#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

class BitcoinExchange {
    private:
        std::map<std::string, double> _db;

        bool isValidDate(const std::string& date);
        bool isValidValue(const std::string& val, double& v);
        std::string trim(const std::string& s);
        double findClosestRate(const std::string& date);
    public:
        BitcoinExchange();
        BitcoinExchange(const BitcoinExchange& other);
        BitcoinExchange& operator=(const BitcoinExchange& other);
        ~BitcoinExchange();
    
        bool loadDatabase(const std::string& file = "data.csv");
        void processInputFile(const std::string& file);
};

#endif