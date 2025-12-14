#include "WordSorter.h"
namespace textanalyzer { 

void WordSorter::CreateStat(const std::map<std::string, int>& wordCount, int numWords) {
    sortedWords.clear();
    for (const auto& wordInfo : wordCount) {
        double freq = (wordInfo.second / (double)numWords) * 100;
        sortedWords.push_back(std::make_tuple(wordInfo.first, wordInfo.second, freq));
    }
    sortedWords.sort([](const auto& a, const auto& b) {
        return std::get<1>(a) > std::get<1>(b);  
    });
}

std::list<std::tuple<std::string, int, double>> WordSorter::getSortedWords() const {
    return sortedWords;
}

}