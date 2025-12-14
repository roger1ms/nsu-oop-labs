#include "WordWriter.h"
namespace textanalyzer { 
    
void CSVWriter::writeToFile(std::string& filename, const std::list<std::tuple<std::string, int, double>>& sortedWords) {
    std::ofstream output(filename);
    output << "Word  |  Frequency  |  Frequency (%)" << std::endl;
    for (const auto& item : sortedWords) {
        output << std::get<0>(item) << "  |  "
               << std::get<1>(item) << "  |  " 
               << std::get<2>(item) << "%"
               << std::endl;
    }
    output.close();
}

}