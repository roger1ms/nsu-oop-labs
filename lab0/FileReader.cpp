#include "FileReader.h"

std::list<std::string> FileReader::ReadFile(std::string& filename) {
	std::list<std::string> lines; 
    std::ifstream input(filename);
    std::string line;
    while (std::getline(input, line)) {
        lines.push_back(line);
    }
    input.close();
    return lines;
}