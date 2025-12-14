#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>
#include <string>
#include <stdexcept>
#include <vector>
#include <iterator>
#include "TuplePrint.h"
#include "CSVParser.h"

int main() {
    std::ifstream file("test.csv");
    CSVParser<int, std::string, std::string, double> parser(file, 0);

    // std::ifstream file("data.csv");
    // CSVParser<int, std::string, double> parser(file, 0, ';');
    

    for (auto it = parser.begin(); it != parser.end(); ++it) {
        std::cout << *it << std::endl;
    }
}