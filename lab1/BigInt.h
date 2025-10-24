#pragma once
#include <string>
#include <iostream>

class BigInt {
public:
    bool is_positive;
    unsigned char* digits;
    size_t size;

    BigInt(const std::string& line);
    BigInt(const BigInt& other);
    BigInt(int other);
    BigInt(long int other);
    BigInt(long long int other);
    ~BigInt();

    void CopyLine(const std::string& line);
    BigInt DeleteZero();

    BigInt operator-() const;
    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator*(const BigInt& other) const;
    BigInt operator/(const BigInt& other) const;

    BigInt operator+=(const BigInt& other);
    BigInt operator-=(const BigInt& other);
    BigInt operator*=(const BigInt& other);
    BigInt operator/=(const BigInt& other);

    bool operator<(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;

    BigInt operator=(const BigInt& other);

    operator std::string() const;
    
};
std::ostream& operator<<(std::ostream& os, const BigInt& num);