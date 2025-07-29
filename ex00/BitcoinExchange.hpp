#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

#include <map>
#include <string>
#include <iostream>

class BitcoinExchange
{
    private:
        std::map<std::string, double> _database;
        bool isValidDate(const std::string& date);
        bool isValidValue(const std::string& valueStr, double& value);
        std::string trim(const std::string& str);
        double findClosestRate(const std::string& date);
    public:
        BitcoinExchange();
        BitcoinExchange(const BitcoinExchange& other);
        BitcoinExchange& operator=(const BitcoinExchange& other);
        ~BitcoinExchange();

        bool loadDatabase(const std::string& filename = "data.csv");
        void processInputFile(const std::string& filename);
};

#endif