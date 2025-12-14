#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include <tuple>
#include <string>
#include <vector>
#include "CSVParser.h"

TEST(CSVParserTest, SimpleCSV) {
    std::stringstream ss("1,apple,3.14\n2,banana,2.71\n3,cherry,1.41\n");
    
    CSVParser<int, std::string, double> parser(ss);
    
    std::vector<std::tuple<int, std::string, double>> results;
    for (const auto& row : parser) {
        results.push_back(row);
    }
    
    EXPECT_EQ(results.size(), 3);
    EXPECT_EQ(results[0], std::make_tuple(1, "apple", 3.14));
    EXPECT_EQ(results[1], std::make_tuple(2, "banana", 2.71));
    EXPECT_EQ(results[2], std::make_tuple(3, "cherry", 1.41));
}

TEST(CSVParserTest, WithQuotes) {
    std::stringstream ss("1,\"apple, inc\",3.14\n2,\"banana \"\"split\"\"\",2.71\n");
    
    CSVParser<int, std::string, double> parser(ss);
    
    std::vector<std::tuple<int, std::string, double>> results;
    for (const auto& row : parser) {
        results.push_back(row);
    }
    
    EXPECT_EQ(results.size(), 2);
    EXPECT_EQ(results[0], std::make_tuple(1, "apple, inc", 3.14));
    EXPECT_EQ(results[1], std::make_tuple(2, "banana \"split\"", 2.71));
}

TEST(CSVParserTest, SkipLines) {
    std::stringstream ss("header1,header2,header3\n1,apple,3.14\n2,banana,2.71\n");
    
    CSVParser<int, std::string, double> parser(ss, 1);
    
    std::vector<std::tuple<int, std::string, double>> results;
    for (const auto& row : parser) {
        results.push_back(row);
    }
    
    EXPECT_EQ(results.size(), 2);
    EXPECT_EQ(results[0], std::make_tuple(1, "apple", 3.14));
    EXPECT_EQ(results[1], std::make_tuple(2, "banana", 2.71));
}

TEST(CSVParserTest, DifferentSeparator) {
    std::stringstream ss("1;apple;3.14\n2;banana;2.71\n");
    
    CSVParser<int, std::string, double> parser(ss, 0, ';');
    
    std::vector<std::tuple<int, std::string, double>> results;
    for (const auto& row : parser) {
        results.push_back(row);
    }
    
    EXPECT_EQ(results.size(), 2);
    EXPECT_EQ(results[0], std::make_tuple(1, "apple", 3.14));
    EXPECT_EQ(results[1], std::make_tuple(2, "banana", 2.71));
}

TEST(CSVParserTest, WrongSeparatorThrows) {
    std::stringstream ss("1,apple,3.14\n2,banana,2.71\n");
    
    CSVParser<int, std::string, double> parser(ss, 0, ';');
    
    EXPECT_THROW({
        for (const auto& row : parser) {
            // Просто пытаемся пройтись
        }
    }, CSVFormatError);
}

TEST(CSVParserTest, MalformedCSV) {
    std::stringstream ss("1,apple,3.14\n2,banana\n3,cherry,1.41,extra\n");
    
    CSVParser<int, std::string, double> parser(ss);
    
    std::vector<std::tuple<int, std::string, double>> results;
    
    EXPECT_THROW({
        for (const auto& row : parser) {
            results.push_back(row);
        }
    }, CSVFormatError);
}

TEST(CSVParserTest, EmptyFile) {
    std::stringstream ss("");
    
    CSVParser<int, std::string, double> parser(ss);
    
    std::vector<std::tuple<int, std::string, double>> results;
    for (const auto& row : parser) {
        results.push_back(row);
    }
    
    EXPECT_TRUE(results.empty());
}

TEST(CSVParserTest, UnclosedQuoteThrows) {
    std::stringstream ss("1,\"apple,3.14\n2,banana,2.71\n");
    
    CSVParser<int, std::string, double> parser(ss);
    
    EXPECT_THROW({
        for (const auto& row : parser) {
            // Просто пытаемся пройтись
        }
    }, CSVParseError);
}

TEST(CSVParserTest, ParseErrorInvalidType) {
    std::stringstream ss("1,apple,3.14\n2,banana,not_a_number\n");
    
    CSVParser<int, std::string, double> parser(ss);
    
    std::vector<std::tuple<int, std::string, double>> results;
    
    EXPECT_THROW({
        for (const auto& row : parser) {
            results.push_back(row);
        }
    }, CSVParseError);
}

TEST(CSVParserTest, IteratorOperations) {
    std::stringstream ss("1,apple,3.14\n2,banana,2.71\n3,cherry,1.41\n");
    
    CSVParser<int, std::string, double> parser(ss);
    
    auto it = parser.begin();
    auto end = parser.end();
    
    EXPECT_NE(it, end);
    
    auto row1 = *it;
    EXPECT_EQ(row1, std::make_tuple(1, "apple", 3.14));
    
    ++it;
    auto row2 = *it;
    EXPECT_EQ(row2, std::make_tuple(2, "banana", 2.71));
    
    auto it2 = it++;
    EXPECT_EQ(*it2, std::make_tuple(2, "banana", 2.71));
    EXPECT_EQ(*it, std::make_tuple(3, "cherry", 1.41));
    
    ++it;
    EXPECT_EQ(it, end);
}

TEST(CSVParserTest, RangeBasedFor) {
    std::stringstream ss("1,apple,3.14\n2,banana,2.71\n");
    
    CSVParser<int, std::string, double> parser(ss);
    
    int count = 0;
    for (const auto& [id, name, value] : parser) {
        EXPECT_GT(id, 0);
        EXPECT_FALSE(name.empty());
        EXPECT_GT(value, 0.0);
        count++;
    }
    
    EXPECT_EQ(count, 2);
}

TEST(CSVParserTest, CustomQuoteCharacter) {
    std::stringstream ss("1,'apple, inc',3.14\n2,'banana',2.71\n");
    
    CSVParser<int, std::string, double> parser(ss, 0, ',', '\n', '\'');
    
    std::vector<std::tuple<int, std::string, double>> results;
    for (const auto& row : parser) {
        results.push_back(row);
    }
    
    EXPECT_EQ(results.size(), 2);
    EXPECT_EQ(results[0], std::make_tuple(1, "apple, inc", 3.14));
}