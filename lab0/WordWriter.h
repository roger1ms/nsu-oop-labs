#ifndef WRITER_H
#define WRITER_H

#include <string>
#include <list>
#include <tuple>
#include <fstream>
#include <iostream>
namespace textanalyzer { 
    
class CSVWriter {
public:
    void writeToFile(std::string& filename, const std::list<std::tuple<std::string, int, double>>& sortedWords);
};

}
#endif