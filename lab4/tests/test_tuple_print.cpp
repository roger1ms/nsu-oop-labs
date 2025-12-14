#include <gtest/gtest.h>
#include <tuple>
#include <sstream>
#include <string>
#include "TuplePrint.h"

TEST(TuplePrintTest, EmptyTuple) {
    std::tuple<> empty;
    std::stringstream ss;
    ss << empty;
    EXPECT_EQ(ss.str(), "");
}

TEST(TuplePrintTest, SingleElement) {
    {
        std::tuple<int> t1{42};
        std::stringstream ss;
        ss << t1;
        EXPECT_EQ(ss.str(), "42");
    }
    
    {
        std::tuple<std::string> t2{"hello"};
        std::stringstream ss;
        ss << t2;
        EXPECT_EQ(ss.str(), "hello");
    }
    
    {
        std::tuple<double> t3{3.14159};
        std::stringstream ss;
        ss << t3;
        EXPECT_EQ(ss.str(), "3.14159");
    }
}

TEST(TuplePrintTest, TwoElements) {
    std::tuple<int, std::string> t{42, "hello"};
    std::stringstream ss;
    ss << t;
    EXPECT_EQ(ss.str(), "42 | hello");
}

TEST(TuplePrintTest, MultipleElements) {
    std::tuple<int, std::string, double, bool> t{
        123, "test", 2.718, true
    };
    std::stringstream ss;
    ss << t;
    EXPECT_EQ(ss.str(), "123 | test | 2.718 | 1");
}

TEST(TuplePrintTest, WithSpacesInString) {
    std::tuple<int, std::string, double> t{
        1, "hello world", 3.14
    };
    std::stringstream ss;
    ss << t;
    EXPECT_EQ(ss.str(), "1 | hello world | 3.14");
}

TEST(TuplePrintTest, SpecialCharacters) {
    std::tuple<std::string, std::string> t{"a,b,c", "x|y|z"};
    std::stringstream ss;
    ss << t;
    EXPECT_EQ(ss.str(), "a,b,c | x|y|z");
}

TEST(TuplePrintTest, MoveSemantics) {
    std::tuple<std::string, std::string> t{"move", "me"};
    std::stringstream ss;
    ss << std::move(t);
    EXPECT_EQ(ss.str(), "move | me");
}

TEST(TuplePrintTest, DifferentSeparatorInOutput) {
    std::tuple<int, int, int> t{1, 2, 3};
    std::stringstream ss;
    ss << t;
    EXPECT_EQ(ss.str(), "1 | 2 | 3");
}
