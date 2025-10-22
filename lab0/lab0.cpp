#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include <Windows.h>

class FileReader {
public:
	std::list<std::string> ReadFile(std::string& filename) {
		std::list<std::string> lines; 
		std::ifstream input(filename);
		std::string line;
		while (std::getline(input, line)) {
    		lines.push_back(line);
		}
		input.close();
		return lines;
	}
};

class WordCounter {
public:
	std::map<std::string, int> wordCount;
    int totalWords;
    void countWords(const std::list<std::string>& lines) {
        totalWords = 0;
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
    
    const std::map<std::string, int>& getWordCount() {
        return wordCount;
    }
    
    int getTotalWords() {
        return totalWords;
    }
};

class WordSorter {
public:
	std::list<std::tuple<std::string, int, double>> sortedWords;

	void CreateStat(std::map<std::string, int> wordCount, int numWords) {
		sortedWords.clear();
		for (const auto& wordInfo : wordCount) {
			double freq = (wordInfo.second/(double)numWords)*100;
			sortedWords.push_back(std::make_tuple(wordInfo.first, wordInfo.second, freq));
		}
		sortedWords.sort([](const auto& a, const auto& b) {
			return std::get<1>(a) > std::get<1>(b);  
		});
	}
	
	std::list<std::tuple<std::string, int, double>> getSortedWords() {
		return sortedWords;
	}
};

class CSVWriter {
public:
	void writeToFile(std::string& filename, std::list<std::tuple<std::string, int, double>> sortedWords) {
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
};

class ProgramCollector {
public:
	FileReader reader;
	WordCounter counter;
	WordSorter sorter;
	CSVWriter writer;
	void Make(std::string& inputFile, std::string& outputFile) {
		std::list<std::string> lines = reader.ReadFile(inputFile);

		counter.countWords(lines);
		std::map<std::string, int> wordCount = counter.getWordCount();
		int numWords = counter.getTotalWords();

		sorter.CreateStat(wordCount, numWords);
		std::list<std::tuple<std::string, int, double>> sortedWords = sorter.getSortedWords();

		writer.writeToFile(outputFile, sortedWords);
	}
};

int main(int argc, char* argv[]) {
	int b = 5;
	int a = b;
	b++;
	printf("%d", a);
}