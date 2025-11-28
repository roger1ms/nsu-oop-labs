#include <iostream>
#include "BigInt.h"

void printBigInt(const BigInt& num) {
    if (!num.is_positive) std::cout << "-";
    for (int i = num.size - 1; i >= 0; i--) std::cout << (int)num.digits[i];
    std::cout << std::endl;
}

int main() {
    BigInt a("12345678901234567890");
    BigInt b("10");
    BigInt c = a * b;
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << c;
    return 0;
}
