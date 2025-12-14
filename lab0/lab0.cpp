#include <iostream>
#include <string>
#include <Windows.h>
#include "ProgramCollector.h"

bool ValidateFiles(const std::string& inputFile, const std::string& outputFile) {
    if (inputFile.length() < 4 || inputFile.substr(inputFile.length() - 4) != ".txt") {
        std::cerr << "Error: Input file must have .txt extension" << std::endl;
        return false;
    }
    
    if (outputFile.length() < 4 || outputFile.substr(outputFile.length() - 4) != ".csv") {
        std::cerr << "Error: Output file must have .csv extension" << std::endl;
        return false;
    }
    
    if (inputFile == outputFile) {
        std::cerr << "Error: Input and output files cannot be the same" << std::endl;
        return false;
    }
    
    std::ifstream input(inputFile);
    if (!input.is_open()) {
        std::cerr << "Error: Cannot open input file '" << inputFile << "'" << std::endl;
        return false;
    }
    input.close();
    
    
    std::ofstream output(outputFile);
    if (!output.is_open()) {
        std::cerr << "Error: Cannot create output file '" << outputFile << "'" << std::endl;
        return false;
    }
    output.close();
    
    return true;
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL,"Russian");
    
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input.txt> <output.csv>" << std::endl;
        std::cerr << "Example: " << argv[0] << " input.txt output.csv" << std::endl;
        return 1;
    }
    
    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    
    if (!ValidateFiles(inputFile, outputFile)) {
        return 1;
    }
    
    try {
        textanalyzer::ProgramCollector collector;
        collector.Make(inputFile, outputFile);
        std::cout << "Program completed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}