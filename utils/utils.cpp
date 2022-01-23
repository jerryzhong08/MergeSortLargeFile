#include <exception>
#include <iomanip>
#include <iostream>

#include "utils.hpp"

std::string vectorToString(const std::vector<std::string>& vect) {
    std::stringstream ss;
    std::for_each(vect.begin(), vect.end(), [&ss](const std::string& elem) {
        ss << elem;
    });
    return ss.str();
}

void printTm(const tm& timeTm) {
    std::cout << "Year " << timeTm.tm_year << '\n';
    std::cout << "Month " << timeTm.tm_mon << '\n';
    std::cout << "Day " << timeTm.tm_mday << '\n';
    std::cout << "Hour " << timeTm.tm_hour << '\n';
    std::cout << "Minute " << timeTm.tm_min << '\n';
    std::cout << "Second " << timeTm.tm_sec << '\n';
}

const char* timeInt2Str(int secondsSinceEpoch) {
    time_t t = secondsSinceEpoch;
    const char* t_str = ctime(&t);
    // std::cout << "Seconds " << secondsSinceEpoch << " is converted to " << t_str << '\n';
    return t_str;
}

tm timeStr2Tm(const char* timeStr) {
    std::istringstream iss(timeStr);
    tm time_tm;
    iss >> std::get_time(&time_tm, "%a %b %d %H:%M:%S %Y");
    if (iss.fail()) {
        std::string msg("Failed to parse time ");
        msg.append(timeStr);
        std::cout << msg << '\n';
        throw std::logic_error(msg);
    } else {
        // std::cout << "Parsing " << std::put_time(&time_tm, "%c") << " succeeded.\n";
        // printTm(time_tm);
    }
    return time_tm;
}
