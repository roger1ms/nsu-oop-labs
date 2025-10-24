#include <gtest/gtest.h>
#include "BigInt.h"
#include <sstream>

TEST(BigIntTest, FromStringToBigInt) {
    BigInt a("12345");
    EXPECT_EQ(static_cast<std::string>(a), "12345");

    BigInt b("-987");
    EXPECT_EQ(static_cast<std::string>(b), "-987");
}

TEST(BigIntTest, IsSignIsCorrect) {
    BigInt a("-42");
    EXPECT_FALSE(a.is_positive);

    BigInt b("0");
    EXPECT_TRUE(b.is_positive);
    EXPECT_EQ(static_cast<std::string>(b), "0");
}

TEST(BigIntTest, CheckSize) {
    BigInt a("-10000000000000000000000000000");
    EXPECT_EQ(a.size, 29);
}

TEST(BigIntTest, Overflow) {
    BigInt a = INT_MIN;
    a = -a;
    EXPECT_EQ(static_cast<std::string>(a), "2147483648");
}

TEST(BigIntTest, Addition) {
    BigInt a("999");
    BigInt b("111");
    BigInt c = a + b;
    EXPECT_EQ(static_cast<std::string>(c), "1110");

    BigInt d("-9876");
    BigInt e("19876");
    BigInt f = d + e;
    EXPECT_EQ(static_cast<std::string>(f), "10000");
}

TEST(BigIntTest, Subtraction) {
    BigInt a("1000");
    BigInt b("1");
    BigInt c = a - b;
    EXPECT_EQ(static_cast<std::string>(c), "999");

    BigInt d("-5");
    BigInt e("15");
    BigInt f = d - e;
    EXPECT_EQ(static_cast<std::string>(f), "-20");
}

TEST(BigIntTest, Multiplication) {
    BigInt a("123");
    BigInt b("456");
    BigInt c = a * b;
    EXPECT_EQ(static_cast<std::string>(c), "56088");

    BigInt d("-12345");
    BigInt e("-4214214");
    BigInt f = d * e;
    EXPECT_EQ(static_cast<std::string>(f), "52024471830");
}

TEST(BigIntTest, Division) {
    BigInt a("100");
    BigInt b("10");
    BigInt c = a / b;
    EXPECT_EQ(static_cast<std::string>(c), "10");

    BigInt d("9");
    BigInt e("2");
    BigInt f = d / e;
    EXPECT_EQ(static_cast<std::string>(f), "4");
}

TEST(BigIntTest, Comparison) {
    BigInt a("123");
    BigInt b("456");

    EXPECT_TRUE(a < b);
    EXPECT_TRUE(b > a);
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(b >= a);

    BigInt c("123");
    EXPECT_FALSE(a < c);
    EXPECT_TRUE(a <= c);
    EXPECT_TRUE(a >= c);
}

TEST(BigIntTest, Increment) {
    BigInt a("123");
    a+=1;
    EXPECT_EQ(static_cast<std::string>(a), "124");
}


TEST(BigIntTest, StreamOutput) {
    BigInt a("123456");
    std::ostringstream cout;
    cout << a;
    EXPECT_EQ(cout.str(), "123456");
}
