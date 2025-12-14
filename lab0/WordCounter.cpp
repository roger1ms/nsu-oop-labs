#include "WordCounter.h"
namespace textanalyzer { 

void WordCounter::countWords(const std::list<std::string>& lines) {
    totalWords = 0;
    wordCount.clear();
    
    for (const std::string& line : lines) {
        std::string word;
        for (char c : line) {
            if (std::isalnum(c)) {
                word += c;
            }
            else if (!word.empty()) {
                wordCount[word]++;
                totalWords++;
                word.clear();	
            }
        }
        if (!word.empty()) {
            wordCount[word]++;
            totalWords++;
        }
    }
}

const std::map<std::string, int>& WordCounter::getWordCount() const {
    return wordCount;
}

int WordCounter::getTotalWords() const {
    return totalWords;
}

}