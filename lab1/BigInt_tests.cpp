#include <gtest/gtest.h>
#include "BigInt.h"
#include <sstream>
#include <climits>

TEST(BigIntTest, FromStringToBigInt) {
    BigInt a("12345");
    EXPECT_EQ(static_cast<std::string>(a), "12345");

    BigInt b("-987");
    EXPECT_EQ(static_cast<std::string>(b), "-987");
}

TEST(BigIntTest, FromIntConstructor) {
    BigInt a(42);
    EXPECT_EQ(static_cast<std::string>(a), "42");

    BigInt b(-100);
    EXPECT_EQ(static_cast<std::string>(b), "-100");
}

TEST(BigIntTest, IsSignIsCorrect) {
    BigInt a("-42");
    EXPECT_FALSE(a.is_positive);

    BigInt b("0");
    EXPECT_TRUE(b.is_positive);
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

// --- Арифметические операции ---

TEST(BigIntTest, AdditionPositive) {
    BigInt a("999");
    BigInt b("111");
    BigInt c = a + b;
    EXPECT_EQ(static_cast<std::string>(c), "1110");
}

TEST(BigIntTest, AdditionDifferentSigns) {
    BigInt d("-9876");
    BigInt e("19876");
    BigInt f = d + e;
    EXPECT_EQ(static_cast<std::string>(f), "10000");
}

TEST(BigIntTest, AdditionWithZero) {
    BigInt a("12345");
    BigInt b("0");
    EXPECT_EQ(static_cast<std::string>(a + b), "12345");
}

TEST(BigIntTest, SubtractionPositive) {
    BigInt a("1000");
    BigInt b("1");
    BigInt c = a - b;
    EXPECT_EQ(static_cast<std::string>(c), "999");
}

TEST(BigIntTest, SubtractionNegativeResult) {
    BigInt d("-5");
    BigInt e("15");
    BigInt f = d - e;
    EXPECT_EQ(static_cast<std::string>(f), "-20");
}

TEST(BigIntTest, SubtractionToZero) {
    BigInt a("12345");
    BigInt b("12345");
    EXPECT_EQ(static_cast<std::string>(a - b), "0");
}

TEST(BigIntTest, MultiplicationPositive) {
    BigInt a("123");
    BigInt b("456");
    EXPECT_EQ(static_cast<std::string>(a * b), "56088");
}

TEST(BigIntTest, MultiplicationNegative) {
    BigInt d("-12345");
    BigInt e("-4214214");
    EXPECT_EQ(static_cast<std::string>(d * e), "52024471830");
}

TEST(BigIntTest, MultiplicationWithZero) {
    BigInt a("123456789");
    BigInt b("0");
    EXPECT_EQ(static_cast<std::string>(a * b), "0");
}

TEST(BigIntTest, DivisionSimple) {
    BigInt a("100");
    BigInt b("10");
    EXPECT_EQ(static_cast<std::string>(a / b), "10");
}

TEST(BigIntTest, DivisionWithRemainder) {
    BigInt d("9");
    BigInt e("2");
    EXPECT_EQ(static_cast<std::string>(d / e), "4");
}

TEST(BigIntTest, DivisionByOne) {
    BigInt a("123456789");
    BigInt b("1");
    EXPECT_EQ(static_cast<std::string>(a / b), "123456789");
}

TEST(BigIntTest, DivisionBySelf) {
    BigInt a("999");
    EXPECT_EQ(static_cast<std::string>(a / a), "1");
}

// --- Проверка сравнения ---

TEST(BigIntTest, ComparisonOperators) {
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

TEST(BigIntTest, ComparisonNegativePositive) {
    BigInt a("-1");
    BigInt b("1");
    EXPECT_TRUE(a < b);
    EXPECT_TRUE(b > a);
}

// --- Инкремент и декремент ---

TEST(BigIntTest, IncrementOperator) {
    BigInt a("123");
    a += 1;
    EXPECT_EQ(static_cast<std::string>(a), "124");
}

TEST(BigIntTest, DecrementOperator) {
    BigInt a("0");
    a -= 1;
    EXPECT_EQ(static_cast<std::string>(a), "-1");
}

// --- Унарные операции и знаки ---

TEST(BigIntTest, Negation) {
    BigInt a("42");
    BigInt b = -a;
    EXPECT_EQ(static_cast<std::string>(b), "-42");
}


// --- Потоки и строки ---

TEST(BigIntTest, StreamOutput) {
    BigInt a("123456");
    std::ostringstream cout;
    cout << a;
    EXPECT_EQ(cout.str(), "123456");
}

TEST(BigIntTest, StringCast) {
    BigInt a("-99999");
    std::string s = static_cast<std::string>(a);
    EXPECT_EQ(s, "-99999");
}

TEST(BigIntTest, CopyConstructor) {
    BigInt a("12345");
    BigInt b(a);
    EXPECT_EQ(static_cast<std::string>(b), "12345");
}

TEST(BigIntTest, AssignmentOperator) {
    BigInt a("111");
    BigInt b("222");
    a = b;
    EXPECT_EQ(static_cast<std::string>(a), "222");
}

// --- Тесты переполнений и больших чисел ---

TEST(BigIntTest, BigAddition_NoOverflow) {
    BigInt a("9999999999999999999999999999999999");
    BigInt b("1");
    BigInt c = a + b;
    EXPECT_EQ(static_cast<std::string>(c), "10000000000000000000000000000000000");
}

TEST(BigIntTest, BigAddition_LargeBoth) {
    BigInt a("9999999999999999999999999999999999");
    BigInt b("9999999999999999999999999999999999");
    BigInt c = a + b;
    EXPECT_EQ(static_cast<std::string>(c), "19999999999999999999999999999999998");
}

TEST(BigIntTest, BigSubtraction_NoUnderflow) {
    BigInt a("10000000000000000000000000000000000");
    BigInt b("9999999999999999999999999999999999");
    BigInt c = a - b;
    EXPECT_EQ(static_cast<std::string>(c), "1");
}

TEST(BigIntTest, BigMultiplication_NoOverflow) {
    BigInt a("9999999999999999");
    BigInt b("9999999999999999");
    BigInt c = a * b;
    EXPECT_EQ(static_cast<std::string>(c), "99999999999999980000000000000001");
}

TEST(BigIntTest, BigMultiplication_ByTen) {
    BigInt a("12345678901234567890");
    BigInt b("10");
    BigInt c = a * b;
    EXPECT_EQ(static_cast<std::string>(c), "123456789012345678900");
}

TEST(BigIntTest, BigDivision_ByTen) {
    BigInt a("12345678901234567890");
    BigInt b("10");
    BigInt c = a / b;
    EXPECT_EQ(static_cast<std::string>(c), "1234567890123456789");
}

TEST(BigIntTest, BigDivision_ByLargeNumber) {
    BigInt a("10000000000000000000000000000000000");
    BigInt b("9999999999999999999999999999999999");
    BigInt c = a / b;
    EXPECT_EQ(static_cast<std::string>(c), "1");
}

TEST(BigIntTest, AdditionCarryOverManyDigits) {
    BigInt a("999999999999999999999999999999");
    BigInt b("999999999999999999999999999999");
    BigInt c = a + b;
    EXPECT_EQ(static_cast<std::string>(c), "1999999999999999999999999999998");
}

TEST(BigIntTest, SubtractionResultingNegativeLarge) {
    BigInt a("1");
    BigInt b("1000000000000000000000000000000000000");
    BigInt c = a - b;
    EXPECT_EQ(static_cast<std::string>(c), "-999999999999999999999999999999999999");
}

TEST(BigIntTest, MultiplicationLargeNumbersSigns) {
    BigInt a("-9999999999999999");
    BigInt b("9999999999999999");
    BigInt c = a * b;
    EXPECT_EQ(static_cast<std::string>(c), "-99999999999999980000000000000001");
}

TEST(BigIntTest, DivisionLargeNegativeByPositive) {
    BigInt a("-100000000000000000000");
    BigInt b("10");
    BigInt c = a / b;
    EXPECT_EQ(static_cast<std::string>(c), "-10000000000000000000");
}

TEST(BigIntTest, DivisionLargeNegativeByNegative) {
    BigInt a("-100000000000000000000");
    BigInt b("-10");
    BigInt c = a / b;
    EXPECT_EQ(static_cast<std::string>(c), "10000000000000000000");
}

TEST(BigIntTest, AdditionNearLongLongLimit) {
    BigInt a("9223372036854775807");  
    BigInt b("1");
    BigInt c = a + b;
    EXPECT_EQ(static_cast<std::string>(c), "9223372036854775808");
}

TEST(BigIntTest, MultiplicationBeyondLongLong) {
    BigInt a("9223372036854775807");  // LLONG_MAX
    BigInt b("9223372036854775807");
    BigInt c = a * b;
    EXPECT_EQ(static_cast<std::string>(c), "85070591730234615847396907784232501249");
}

TEST(BigIntTest, ManyCarryPropagation) {
    BigInt a("999999999999999999999999");
    BigInt b("9");
    BigInt c = a + b;
    EXPECT_EQ(static_cast<std::string>(c), "1000000000000000000000008");
}
