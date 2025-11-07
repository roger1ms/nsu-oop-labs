#include <iostream>
#include "BigInt.h"

void printBigInt(const BigInt& num) {
    if (!num.is_positive) std::cout << "-";
    for (int i = num.size - 1; i >= 0; i--) std::cout << (int)num.digits[i];
    std::cout << std::endl;
}

int main() {
    BigInt a("12345678901234567890");
    BigInt b("-987654321");
    BigInt c = INT_MIN;
    c = c + 4;
    BigInt d = - c;
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::string s = static_cast<std::string>(a);
    std::cout << "String representation: " << s << std::endl;
    return 0;
}
