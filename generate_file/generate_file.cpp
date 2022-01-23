#include <fstream>
#include <iostream>

#include "../utils/utils.hpp"

const std::unordered_map<std::string, int> LINE_NUMBERS = {
    {"10", 10},
    {"100", 100},
    {"10K", 10000},
    {"1M", 1000000},
    {"100M", 100000000}
};

int main(int argc, char *argv[]) {
    int numOfEntries = argc >= 2 ? LINE_NUMBERS.at(argv[1]) : 100;

    int lowerBound = 1000000000;
    int upperBound = 1650000000;
    int diff = upperBound - lowerBound;
    std::cout << "lower bound: " << timeInt2Str(lowerBound) << '\n';
    std::cout << "upper bound: " << timeInt2Str(upperBound) << '\n';
    std::srand(std::time(nullptr));

    std::ofstream fileToSort;
    try {
        fileToSort.open("../data/input/file_to_sort.txt");
        for (int i = 1; i <= numOfEntries; ++i) {
            int random_number = std::rand() % diff + lowerBound;
            fileToSort << i << '|' << timeInt2Str(random_number);
        }
    }
    catch (std::exception& e) {
        std::cout << "Exception caught: " << e.what() << '\n';
    }

    fileToSort.close();

    return 0;
}