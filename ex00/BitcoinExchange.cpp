#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _db(other._db) {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
    if (this != &other) {
        _db = other._db;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

std::string BitcoinExchange::trim(const std::string& s) { //Trim whitespaces
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

bool BitcoinExchange::isValidDate(const std::string& date) {
    if (date.length() != 10)
        return false;
    if (date[4] != '-' || date[7] != '-')
        return false;

    for (size_t i = 0; i < date.length(); ++i) {
        if (i == 4 || i == 7)
            continue;
        if (!std::isdigit(date[i]))
            return false;
    }

    int y = std::atoi(date.substr(0, 4).c_str());
    int m = std::atoi(date.substr(5, 2).c_str());
    int d = std::atoi(date.substr(8, 2).c_str());

    if (y < 1000 || y > 9999)
        return false;
    if (m < 1 || m > 12)
        return false;
    if (d < 1 || d > 31)
        return false;

    if (m == 2) {
        bool isLeap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
        if (d > (isLeap ? 29 : 28))
            return false;
    } else if (m == 4 || m == 6 || m == 9 || m == 11) {
        if (d > 30)
            return false;
    }
    return true;
}

bool BitcoinExchange::isValidValue(const std::string& val, double& v) {
    char* end;
    v = std::strtod(val.c_str(), &end);
    if (*end != '\0')
        return false;
    if (v < 0 || v > 1000)
        return false;
    return true;
}

double BitcoinExchange::findClosestRate(const std::string& date) {
    std::map<std::string, double>::iterator it = _db.upper_bound(date);
}