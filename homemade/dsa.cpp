#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

// 假设已经通过合适的方法获得了以下全局变量
bigint p, q, g, x; // DSA参数，其中p、q为大素数，g为基点，x为私钥

bigint mod_exp(bigint base, bigint exp, bigint mod) {
    // 实现快速模幂运算，如平方-乘法算法
}

bigint hash_function(string message) {
    // 使用合适的哈希函数，简化起见这里不具体实现
}

pair<bigint, bigint> sign(string message) {
    srand(time(0)); // 现实中应使用密码学安全的随机数生成器
    bigint k = rand() % (q - 1) + 1; // 生成随机数k
    bigint r = mod_exp(g, k, p) % q; // 计算r
    if (r == 0) return sign(message); // 防止r为0的情况
    bigint k_inv = mod_exp(k, q - 2, q); // 计算k的模逆元
    bigint Hm = hash_function(message);
    bigint s = (k_inv * (Hm + x * r)) % q; // 计算s
    if (s == 0) return sign(message); // 防止s为0的情况
    return make_pair(r, s);
}

bool verify(string message, pair<bigint, bigint> signature) {
    bigint r = signature.first;
    bigint s = signature.second;
    if (r < 1 || r >= q || s < 1 || s >= q) return false;
    bigint Hm = hash_function(message);
    bigint w = mod_exp(s, q - 2, q); // 计算w
    bigint u1 = (Hm * w) % q;
    bigint u2 = (r * w) % q;
    bigint v = ((mod_exp(g, u1, p) * mod_exp(y, u2, p)) % p) % q; // 计算v
    return v == r;
}