#ifndef COUNTER_H
#define COUNTER_H

#include <string>
#include <list>
#include <map>
namespace textanalyzer { 

class WordCounter {
private:
    std::map<std::string, int> wordCount;
    int totalWords;
    
public:
    WordCounter() : totalWords(0) {}
    void countWords(const std::list<std::string>& lines);
    const std::map<std::string, int>& getWordCount() const;
    int getTotalWords() const;
    
};

}
#endif