#include <iostream>
#include "BigInt.h"

void printBigInt(const BigInt& num) {
    if (!num.is_positive) std::cout << "-";
    for (int i = num.size - 1; i >= 0; i--) std::cout << (int)num.digits[i];
    std::cout << std::endl;
}

int main() {
    BigInt a = 100;
    BigInt b = 3;
    BigInt c = a + b;

    std::cout << "a = "; printBigInt(a);
    std::cout << "b = "; printBigInt(b);
    std::cout << "c = a + b = "; printBigInt(c);

    return 0;
}
