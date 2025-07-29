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
    if (it == _db.begin())
        return -1;
    --it;
    return it->second;
}

bool BitcoinExchange::loadDatabase(const std::string& file) {
    std::ifstream f(file.c_str());
    if (!f.is_open()) {
        std::cerr << "Error: could not open database file." << std::endl;
        return false;
    }

    std::string line;
    bool first = true;

    while (std::getline(f, line)) {
        if (first) {
            first = false;
            continue;
        }
        size_t pos = line.find(',');
        if (pos == std::string::npos)
            continue;
        std::string date = trim(line.substr(0, pos));
        std::string val = trim(line.substr(pos + 1));
        if (isValidDate(date)) {
            double v = std::strtod(val.c_str(), NULL);
            _db[date] = v;
        }
    }
    f.close();
    return !_db.empty();
}

void BitcoinExchange::processInputFile(const std::string& file) {
    std::ifstream f(file.c_str());
    if (!f.is_open()) {
        std::cerr << "Error: could not open file." << std::endl;
        return;
    }
    
    std::string line;
    bool first = true;
    
    while (std::getline(f, line)) {
        if (first) {
            first = false;
            if (line.find("date") != std::string::npos && line.find("value") != std::string::npos) {
                continue;
            }
        }
        size_t pos = line.find('|');
        if (pos == std::string::npos) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }
        std::string dateStr = trim(line.substr(0, pos));
        std::string valStr = trim(line.substr(pos + 1));
        if (!isValidDate(dateStr)) {
            std::cerr << "Error: bad input => " << dateStr << std::endl;
            continue;
        }
        double v;
        if (!isValidValue(valStr, v)) {
            if (v < 0) {
                std::cerr << "Error: not a positive number." << std::endl;
            } else if (v > 1000) {
                std::cerr << "Error: too large a number." << std::endl;
            } else {
                std::cerr << "Error: invalid number format." << std::endl;
            }
            continue;
        }
        double rate = findClosestRate(dateStr);
        if (rate < 0) {
            std::cerr << "Error: no exchange rate available for date." << std::endl;
            continue;
        }
        double result = v * rate;
        std::cout << dateStr << " => " << v << " = " << result << std::endl;
    }
    f.close();
}
