#pragma once
#include <string>

class BigInt {
public:
    bool is_positive;
    unsigned char* digits;
    size_t size;

    BigInt(const std::string& line);
    BigInt(const BigInt& other);
    BigInt(int other);
    ~BigInt();

    void CopyLine(const std::string& line);
    BigInt& DeleteZero();

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

    template <typename T>
    BigInt operator+(T other) const { return *this + BigInt(std::to_string(other)); }

    template <typename T>
    BigInt operator-(T other) const { return *this - BigInt(std::to_string(other)); }

    template <typename T>
    BigInt operator*(T other) const { return *this * BigInt(std::to_string(other)); }

    template <typename T>
    bool operator<(T other) const { return *this < BigInt(std::to_string(other)); }

    template <typename T>
    bool operator>(T other) const { return *this > BigInt(std::to_string(other)); }

    template <typename T>
    bool operator<=(T other) const { return *this <= BigInt(std::to_string(other)); }

    template <typename T>
    bool operator>=(T other) const { return *this >= BigInt(std::to_string(other)); }

    template <typename T>
    BigInt operator+=(T other) { return *this += BigInt(std::to_string(other)); }
    
    template <typename T>
    BigInt operator-=(T other) { return *this -= BigInt(std::to_string(other)); }
    
    template <typename T>
    BigInt operator*=(T other) { return *this *= BigInt(std::to_string(other)); }
    
    template <typename T>
    BigInt operator/=(T other) { return *this /= BigInt(std::to_string(other)); }
};
