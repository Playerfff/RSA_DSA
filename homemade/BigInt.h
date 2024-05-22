//
// Created by huawei on 2024/5/23.
//

#ifndef RSA_DSA_BIGINT_H
#define RSA_DSA_BIGINT_H

#include <algorithm>
#include <compare>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

class BigInt {
public:
    // 构造函数
    BigInt() : digits{0} {}
    BigInt(const std::string &num);
    BigInt(long long num);

    // 运算符重载
    BigInt operator+(const BigInt &rhs) const;
    BigInt operator-(const BigInt &rhs) const;
    BigInt operator*(const BigInt &rhs) const;
    BigInt operator/(const BigInt &rhs) const;
    BigInt operator%(const BigInt &rhs) const;
    BigInt operator-() const;// 一元负号运算符
    BigInt &operator=(const BigInt &rhs);

    // 三向比较运算符重载
    std::strong_ordering operator<=>(const BigInt &rhs) const = default;
    bool operator==(const BigInt &rhs) const = default;

    // 输入输出运算符重载
    friend std::ostream &operator<<(std::ostream &os, const BigInt &num);
    friend std::istream &operator>>(std::istream &is, BigInt &num);

private:
    std::vector<int> digits; // 数字存储（每个元素为一位）
    bool is_negative = false;// 负数标志

    void remove_leading_zeros();
};

#endif//RSA_DSA_BIGINT_H
