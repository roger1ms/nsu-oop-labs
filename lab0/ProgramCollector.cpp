#include "ProgramCollector.h"
namespace textanalyzer { 
void ProgramCollector::Make(std::string& inputFile, std::string& outputFile) {
    std::list<std::string> lines = reader.ReadFile(inputFile);

    counter.countWords(lines);
    std::map<std::string, int> wordCount = counter.getWordCount();
    int numWords = counter.getTotalWords();

    sorter.CreateStat(wordCount, numWords);
    std::list<std::tuple<std::string, int, double>> sortedWords = sorter.getSortedWords();

    writer.writeToFile(outputFile, sortedWords);
}
}