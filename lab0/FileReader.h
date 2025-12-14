#ifndef READER_H
#define READER_H

#include <string>
#include <list>
#include <fstream>

class FileReader {
public:
    std::list<std::string> ReadFile(std::string& filename);
};

#endif