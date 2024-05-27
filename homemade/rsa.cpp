#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <tuple>

// 使用 筛选法 判断给定整数n是否为质数
bool is_prime(int n) {
    if (n <= 1) return false; // 小于等于1的数不是质数
    if (n <= 3) return true;  // 2和3是质数
    if (n % 2 == 0 || n % 3 == 0) return false; // 排除能被2或3整除的数
    for (int i = 5; i * i <= n; i += 6) { // 只检查6k±1形式的数
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

// 在指定范围内生成一个质数
int generate_prime(int min, int max) {
    std::random_device rd; // 获取随机数种子
    std::mt19937 gen(rd()); // 使用Mersenne Twister引擎生成随机数
    std::uniform_int_distribution<> dis(min, max); // 定义均匀分布
    int prime;
    do {
        prime = dis(gen); // 生成随机数
    } while (!is_prime(prime)); // 直到找到质数为止
    return prime;
}

// 欧几里得算法,计算两个整数的最大公约数
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// 扩展欧几里得算法，返回ax + by = gcd(a, b)(裴属定理)的解
std::tuple<int, int, int> extended_gcd(int a, int b) {
    if (b == 0) return std::make_tuple(a, 1, 0);
    auto [g, x1, y1] = extended_gcd(b, a % b);
    int x = y1;
    int y = x1 - (a / b) * y1;
    return std::make_tuple(g, x, y);
}

// 计算模逆元，即求e关于phi的乘法逆元
int mod_inverse(int e, int phi) {
    auto [g, x, y] = extended_gcd(e, phi);
    if (g != 1) throw std::runtime_error("不存在模逆元");
    return (x % phi + phi) % phi; // 确保结果为正
}

// 快速幂计算 (base^exp) % mod
long long mod_pow(long long base, long long exp, long long mod) {
    long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

// RSA密钥对结构体
struct RSAKeyPair {
    int e, d, n; // 公钥指数、私钥指数、模数
};

// 生成RSA密钥对
RSAKeyPair generate_rsa_keys(int bit_length) {
    int min = 1 << (bit_length / 2 - 1);
    int max = 1 << (bit_length / 2);

    int p = generate_prime(min, max); // 生成大质数p
    int q = generate_prime(min, max); // 生成大质数q
    int n = p * q;                   // 计算模数n
    int phi = (p - 1) * (q - 1);     // 计算欧拉函数φ(n)

    int e = 65537; // 选择常用的公钥指数e
    if (gcd(e, phi) != 1) throw std::runtime_error("e与φ(n)不互质");

    int d = mod_inverse(e, phi); // 计算私钥指数d
    return {e, d, n};
}

// 加密函数
int encrypt(int message, int e, int n) {
    return mod_pow(message, e, n);
}

// 解密函数
int decrypt(int ciphertext, int d, int n) {
    return mod_pow(ciphertext, d, n);
}

// 主函数，演示RSA加密解密过程
int main() {
    int bit_length = 16; // 实际场景下应使用至少2048位以保证安全性
    RSAKeyPair keys = generate_rsa_keys(bit_length);

    std::cout << "Public Key (e, n): (" << keys.e << ", " << keys.n << ")\n";
    std::cout << "Private Key (d, n): (" << keys.d << ", " << keys.n << ")\n";

    int message = 42; // 待加密的消息
    int encrypted_message = encrypt(message, keys.e, keys.n);
    int decrypted_message = decrypt(encrypted_message, keys.d, keys.n);

    std::cout << "Original Message: " << message << "\n";
    std::cout << "Encrypted Message: " << encrypted_message << "\n";
    std::cout << "Decrypted Message: " << decrypted_message << "\n";

    return 0;
}

