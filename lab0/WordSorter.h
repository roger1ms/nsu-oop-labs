#ifndef SORTER_H
#define SORTER_H

#include <string>
#include <list>
#include <map>
#include <tuple>
namespace textanalyzer { 

class WordSorter {
private:
    std::list<std::tuple<std::string, int, double>> sortedWords;
    
public:
    void CreateStat(const std::map<std::string, int>& wordCount, int numWords);
    std::list<std::tuple<std::string, int, double>> getSortedWords() const;
};

}
#endif