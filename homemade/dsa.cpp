#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

// 假设已经通过合适的方法获得了以下全局变量
bigint p, q, g, x;// DSA参数定义：
                  // p 和 q 是大素数，其中q是p的因子；
                  // g 是一个小的整数，满足g^q % p = 1（模p下的q次方根）；
                  // x 是用户的私钥，选取为[1,q-1]范围内的随机数。

// 实现快速模幂运算，如平方-乘法算法，计算 (base^exp) % mod 的值。
bigint mod_exp(bigint base, bigint exp, bigint mod) {
    // 具体实现细节略，此函数应递归或迭代地减少指数，并利用模运算性质高效计算。
}

// 哈希函数接口声明，简化起见这里不具体实现其内部逻辑。
// 应使用安全的哈希函数，如SHA-256，将输入消息转换成固定长度的大整数。
bigint hash_function(string message) {
    // 实现细节省略，需返回消息的哈希值。
}

// DSA签名函数，接收消息并返回由(r, s)组成的签名对。
pair<bigint, bigint> sign(string message) {
    srand(time(0));                 // 以当前时间作为随机数生成器的种子
    bigint k = rand() % (q - 1) + 1;// 生成[1, q-1]范围内的随机数k

    // 计算 r = (g^k mod p) mod q
    bigint r = mod_exp(g, k, p) % q;
    if (r == 0) return sign(message);// 若r为0，则重新生成k直至r不为0

    // 计算 k_inv 为k在模q下的乘法逆元，即k * k_inv ≡ 1 (mod q)
    bigint k_inv = mod_exp(k, q - 2, q);

    // 计算消息的哈希值 Hm
    bigint Hm = hash_function(message);

    // 计算 s = (k_inv * (Hm + x*r)) % q
    bigint s = (k_inv * (Hm + x * r)) % q;
    if (s == 0) return sign(message);// 若s为0，则重新生成k直至s不为0

    // 返回签名对 (r, s)
    return make_pair(r, s);
}

// DSA验证函数，接收消息、签名对，返回布尔值表示签名是否有效。
bool verify(string message, pair<bigint, bigint> signature) {
    bigint r = signature.first; // 提取签名中的r部分
    bigint s = signature.second;// 提取签名中的s部分
    // 检查r和s是否在有效范围内
    if (r < 1 || r >= q || s < 1 || s >= q) return false;

    // 计算消息的哈希值 Hm
    bigint Hm = hash_function(message);

    // 计算 w = s^-1 mod q，即s的模逆元
    bigint w = mod_exp(s, q - 2, q);

    // 计算 u1 = (Hm * w) % q 和 u2 = (r * w) % q
    bigint u1 = (Hm * w) % q;
    bigint u2 = (r * w) % q;

    // 计算 v = ((g^u1 * y^u2) mod p) mod q，其中y = g^x mod p 是公钥的一部分
    bigint v = ((mod_exp(g, u1, p) * mod_exp(x, u2, p)) % p) % q;

    // 验证签名：如果计算出的v与签名中的r相同，则签名有效
    return v == r;
}