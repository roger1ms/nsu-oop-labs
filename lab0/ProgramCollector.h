#ifndef PROGRAMCOLLECTOR_H
#define PROGRAMCOLLECTOR_H

#include "FileReader.h"
#include "WordCounter.h"
#include "WordSorter.h"
#include "WordWriter.h"
#include <string>

namespace textanalyzer {
    
class ProgramCollector {
private:
    FileReader reader;
    WordCounter counter;
    WordSorter sorter;
    CSVWriter writer;
    
public:
    void Make(std::string& inputFile, std::string& outputFile);
};

}
#endif