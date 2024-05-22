//
// Created by huawei on 2024/5/23.
//


#include "BigInt.h"

BigInt::BigInt(const std::string &num) {
    is_negative = num[0] == '-';
    for (int i = is_negative ? 1 : 0; i < num.size(); ++i) {
        if (!isdigit(num[i])) throw std::invalid_argument("Invalid number string");
        digits.push_back(num[i] - '0');
    }
    std::reverse(digits.begin(), digits.end());
    remove_leading_zeros();
}

BigInt::BigInt(long long num) {
    is_negative = num < 0;
    num = std::abs(num);
    if (num == 0) {
        digits.push_back(0);
    } else {
        while (num > 0) {
            digits.push_back(num % 10);
            num /= 10;
        }
    }
}

void BigInt::remove_leading_zeros() {
    while (digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
    }
}

BigInt BigInt::operator-() const {
    BigInt result = *this;
    result.is_negative = !is_negative;
    return result;
}

BigInt BigInt::operator+(const BigInt &rhs) const {
    if (is_negative == rhs.is_negative) {
        BigInt result;
        result.is_negative = is_negative;
        int carry = 0;
        for (size_t i = 0; i < std::max(digits.size(), rhs.digits.size()) || carry; ++i) {
            if (i == result.digits.size()) result.digits.push_back(0);
            result.digits[i] += carry + (i < digits.size() ? digits[i] : 0) + (i < rhs.digits.size() ? rhs.digits[i] : 0);
            carry = result.digits[i] >= 10;
            if (carry) result.digits[i] -= 10;
        }
        return result;
    }
    return *this - (-rhs);
}

BigInt BigInt::operator-(const BigInt &rhs) const {
    if (is_negative != rhs.is_negative) {
        return *this + (-rhs);
    }
    if (*this < rhs) {
        return -(rhs - *this);
    }
    BigInt result;
    result.is_negative = is_negative;
    int carry = 0;
    for (size_t i = 0; i < digits.size(); ++i) {
        result.digits.push_back(digits[i] - carry - (i < rhs.digits.size() ? rhs.digits[i] : 0));
        if (result.digits.back() < 0) {
            result.digits.back() += 10;
            carry = 1;
        } else {
            carry = 0;
        }
    }
    result.remove_leading_zeros();
    return result;
}

BigInt BigInt::operator*(const BigInt &rhs) const {
    BigInt result;
    result.digits.resize(digits.size() + rhs.digits.size(), 0);
    for (size_t i = 0; i < digits.size(); ++i) {
        int carry = 0;
        for (size_t j = 0; j < rhs.digits.size() || carry; ++j) {
            long long current = result.digits[i + j] +
                                digits[i] * 1LL * (j < rhs.digits.size() ? rhs.digits[j] : 0) + carry;
            result.digits[i + j] = static_cast<int>(current % 10);
            carry = static_cast<int>(current / 10);
        }
    }
    result.is_negative = is_negative != rhs.is_negative;
    result.remove_leading_zeros();
    return result;
}


BigInt BigInt::operator/(const BigInt &rhs) const {
    if (rhs == 0) throw std::invalid_argument("Division by zero");
    BigInt result, current;
    result.digits.resize(digits.size());
    for (int i = digits.size() - 1; i >= 0; --i) {
        current.digits.insert(current.digits.begin(), digits[i]);
        current.remove_leading_zeros();
        int x = 0, l = 0, r = 10;
        while (l <= r) {
            int m = (l + r) / 2;
            BigInt t = rhs * m;
            if (t <= current) {
                x = m;
                l = m + 1;
            } else {
                r = m - 1;
            }
        }
        result.digits[i] = x;
        current = current - rhs * x;
    }
    result.is_negative = is_negative != rhs.is_negative;
    result.remove_leading_zeros();
    return result;
}

BigInt BigInt::operator%(const BigInt &rhs) const {
    return *this - (*this / rhs) * rhs;
}


BigInt &BigInt::operator=(const BigInt &rhs) {
    if (this == &rhs) return *this;
    digits = rhs.digits;
    is_negative = rhs.is_negative;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const BigInt &num) {
    if (num.is_negative) os << '-';
    for (auto it = num.digits.rbegin(); it != num.digits.rend(); ++it) {
        os << *it;
    }
    return os;
}

std::istream &operator>>(std::istream &is, BigInt &num) {
    std::string str;
    is >> str;
    num = BigInt(str);
    return is;
}

//// 测试函数
//int main() {
//    BigInt num1("12345678901234567890");
//    BigInt num2("98765432109876543210");
//    BigInt sum = num1 + num2;
//    BigInt diff = num1 - num2;
//    BigInt prod = num1 * num2;
//    BigInt quot = num1 / num2;
//    BigInt mod = num1 % num2;
//
//    std::cout << "Sum: " << sum << std::endl;
//    std::cout << "Difference: " << diff << std::endl;
//    std::cout << "Product: " << prod << std::endl;
//    std::cout << "Quotient: " << quot << std::endl;
//    std::cout << "Modulus: " << mod << std::endl;
//    std::cout << "Exponentiation: " << exp << std::endl;
//
//    return 0;
//}
