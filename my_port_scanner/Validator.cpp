#include "Validator.h"

bool isValidIP(const std::string& ip){
    std::vector<int> octets;
    std::stringstream ss(ip);
    std::string octet;
    int count = 0;

    while (std::getline(ss, octet, '.')) {
        count++;
        if (count > 4) return false;

        if (octet.empty() || octet.length() > 3) return false;
        for (char c : octet) {
            if (!isdigit(c)) return false;
        }

        int value = std::stoi(octet);
        if (value < 0 || value > 255) return false;

        if (octet.length() > 1 && octet[0] == '0') return false;

        octets.push_back(value);
    }

    return count == 4;
}