#include "BigInt.h"

BigInt::BigInt(const std::string& line) {
    if (line.empty()) {
        is_positive = true;
        size = 1;
        digits = new unsigned char[1]{0};
        return;
    }
    if (line[0] == '-') {
        is_positive = false;
        size = line.length() - 1;
    } 
    else {
        is_positive = true;
        size = line.length();
    }

    digits = new unsigned char[size];
    std::string num_part = (is_positive ? line : line.substr(1));
    CopyLine(num_part);
}

BigInt::BigInt(const BigInt& other) {
    is_positive = other.is_positive;
    size = other.size;
    digits = new unsigned char[size];
    for (size_t i = 0; i < size; ++i) digits[i] = other.digits[i];
}

BigInt::BigInt(int number) {
    BigInt temp = BigInt(std::to_string(number));
    is_positive = temp.is_positive;
    size = temp.size;
    digits = new unsigned char[size];
    for (size_t i = 0; i < size; ++i) {
        digits[i] = temp.digits[i];
    }
}

BigInt::BigInt(long int number) {
    if (number < 0) {
        is_positive = false;
        number = -number;
    } else {
        is_positive = true;
    }

    if (number == 0) {
        size = 1;
        digits = new unsigned char[1]{0};
        return;
    }
    int temp = number;
    size = 0;
    while (temp > 0) {
        temp /= 10;
        size++;
    }
    digits = new unsigned char[size];
    for (size_t i = 0; i < size; ++i) {
        digits[i] = number % 10;
        number /= 10;
    }
}

BigInt::BigInt(long long int number) {
    if (number < 0) {
        is_positive = false;
        number = -number;
    } else {
        is_positive = true;
    }

    if (number == 0) {
        size = 1;
        digits = new unsigned char[1]{0};
        return;
    }
    int temp = number;
    size = 0;
    while (temp > 0) {
        temp /= 10;
        size++;
    }
    digits = new unsigned char[size];
    for (size_t i = 0; i < size; ++i) {
        digits[i] = number % 10;
        number /= 10;
    }
}

BigInt::~BigInt() {
    delete[] digits;
}

void BigInt::CopyLine(const std::string& line) {
    for (size_t i = 0; i < size; i++)
        digits[i] = line[line.length() - 1 - i] - '0';
}

BigInt BigInt::DeleteZero() {
    while (digits[size - 1] == 0 && size > 1)
        size--;
    return *this;
}

BigInt BigInt::operator-() const {
    BigInt result(*this);
    result.is_positive = !result.is_positive;
    return result;
}

BigInt BigInt::operator+(const BigInt& second_digit) const {
    if (!is_positive && second_digit.is_positive) return second_digit - -(*this);
    if (is_positive && !second_digit.is_positive) return (*this) - second_digit;
    if (!is_positive && !second_digit.is_positive) return -(-(*this) + (-second_digit));
    BigInt a = *this;
    BigInt b = second_digit;

    if (a < b) {
        a = second_digit;
        b = *this;
    }

    size_t max_size = a.size;
    int remain = 0;
    unsigned char* result_digits = new unsigned char[max_size + 1]{0};
    for (size_t i = 0; i < max_size; ++i) {
        int sum = a.digits[i] + (i < b.size ? b.digits[i] : 0) + remain;
        if (sum > 9) { sum -= 10; remain = 1; } else remain = 0;
        result_digits[i] = sum;
    }
    size_t result_size = max_size;
    if (remain) { result_digits[max_size] = 1; result_size++; }

    BigInt result("0");
    delete[] result.digits;
    result.digits = result_digits;
    result.size = result_size;
    result.is_positive = true;
    result.DeleteZero();
    return result;
}

BigInt BigInt::operator-(const BigInt& second_digit) const {
    if (!is_positive && second_digit.is_positive) return -(-(*this) + second_digit);
    if (is_positive && !second_digit.is_positive) return (*this) + (-second_digit);
    if (!is_positive && !second_digit.is_positive) return -(-(*this) - (-second_digit));

    BigInt bigger = (*this);
    BigInt smaller = second_digit;
    if (bigger < smaller) {
        bigger = second_digit;
        smaller = (*this);
        bigger.is_positive = false;
    }

    int remain = 0;
    for (size_t i = 0; i < bigger.size; ++i) {
        int sub = bigger.digits[i] - (i < smaller.size ? smaller.digits[i] : 0) - remain;
        if (sub < 0) { sub += 10; remain = 1; } else remain = 0;
        bigger.digits[i] = sub;
    }
    bigger.DeleteZero();
    return bigger;
}

BigInt BigInt::operator*(const BigInt& second_digit) const {
    BigInt a = *this;
    BigInt b = second_digit;
    if (a < b) {
        a = second_digit;
        b = *this;
    }
    size_t result_size = a.size + b.size;
    unsigned char* result_digits = new unsigned char[result_size]{0};

    for (size_t i = 0; i < b.size; i++)
        for (size_t j = 0; j < a.size; j++) {
            int composition = b.digits[i] * a.digits[j];
            result_digits[i + j] += composition;
        }

    for (size_t k = 0; k < result_size - 1; k++) {
        result_digits[k + 1] += result_digits[k] / 10;
        result_digits[k] %= 10;
    }

    BigInt result("");
    delete[] result.digits;
    result.digits = result_digits;
    result.size = result_size;
    result.is_positive = (is_positive == second_digit.is_positive);
    result.DeleteZero();
    return result;
}

BigInt BigInt::operator/(const BigInt& second_digit) const {
    BigInt a = *this;
    a.is_positive = true;
    BigInt b = second_digit;
    b.is_positive = true;
    if (a < second_digit) {
        BigInt result("0");
        return result;
    }
    size_t result_size = a.size;
    unsigned char* result_digits = new unsigned char[result_size]{0};
    BigInt remainder("0");
    int  cnt = 0;
    for (int i = a.size - 1; i >= 0; i--) {
        remainder = remainder * 10 + BigInt(a.digits[i]);
        if (remainder >= b) {
            int j = 1;
            BigInt bj = b;
            while (remainder >= bj) {
                bj = bj + b;
                j++;
            }
            remainder = remainder - (bj - b);
            result_digits[cnt++] = j-1;
        }
        else {
            result_digits[cnt++] = 0;
        }
    }
    for (int i = 0; i < cnt/2; i++) {
        int swap = result_digits[i];
        result_digits[i] = result_digits[cnt-1-i];
        result_digits[cnt-1-i] = swap;
    }
    BigInt result("");
    delete[] result.digits;
    result.digits = result_digits;
    result.size = cnt; 
    result.is_positive = (is_positive == second_digit.is_positive);
    result.DeleteZero();
    return result;
}

BigInt BigInt::operator+=(const BigInt& other) {
    *this = *this + other;
    return *this;
}

BigInt BigInt:: operator-=(const BigInt& other) {
    *this = *this - other;
    return *this;
}

BigInt BigInt:: operator*=(const BigInt& other) {
    *this = *this * other;
    return *this;
}

BigInt BigInt:: operator/=(const BigInt& other) {
    *this = *this / other;
    return *this;
}

bool BigInt::operator<(const BigInt& other) const {
    if (is_positive && !other.is_positive) return false;
    if (!is_positive && other.is_positive) return true;

    if (is_positive) {
        if (size != other.size) return size < other.size;
        for (int i = size - 1; i >= 0; i--)
            if (digits[i] != other.digits[i]) return digits[i] < other.digits[i];
        return false;
    } 
    else {
        if (size != other.size) return size > other.size;
        for (int i = size - 1; i >= 0; --i)
            if (digits[i] != other.digits[i]) return digits[i] > other.digits[i];
        return false;
    }
}
bool BigInt::operator>(const BigInt& other) const { return other < (*this); }
bool BigInt::operator<=(const BigInt& other) const { return !(*this > other); }
bool BigInt::operator>=(const BigInt& other) const { return !(*this < other); }

BigInt BigInt::operator=(const BigInt& other) {
    if (this == &other) return *this;
    delete[] digits;
    size = other.size;
    is_positive = other.is_positive;
    digits = new unsigned char[size];
    for (size_t i = 0; i < size; ++i) digits[i] = other.digits[i];
    return *this;
}

BigInt::operator std::string() const {
    std::string str;
    if (!is_positive && !(size == 1 && digits[0] == 0))
        str.push_back('-');
    for (int i = size - 1; i >= 0; --i)
        str.push_back('0' + digits[i]);
    return str;
}

std::ostream& operator<<(std::ostream& cout, const BigInt& num) {
    cout << static_cast<std::string>(num);
    return cout;
}