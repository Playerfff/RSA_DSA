#include <iostream>

// 判断一个数是否为素数
bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

// 求模反元素
int modInverse(int a, int m) {
    int m0 = m, t, q;
    int x0 = 0, x1 = 1;

    if (m == 1) return 0;

    // 应用扩展欧几里得算法
    while (a > 1) {
        // q 是商， t 是余数
        q = a / m;
        t = m;

        // m 是余数， a 是除数
        m = a % m;
        a = t;

        // 更新 x 和 y
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    // 保证 x1 为正数
    if (x1 < 0) x1 += m0;
    return x1;
}

// 快速幂取模运算
int powerMod(int base, int exp, int mod) {
    if (exp == 0) return 1;
    int half = powerMod(base, exp / 2, mod);
    int half_mod = (1LL * half * half) % mod;
    if (exp % 2 != 0) {
        return (1LL * half_mod * base) % mod;
    } else {
        return half_mod;
    }
}

// RSA 加密
int encrypt(int msg, int e, int n) {
    return powerMod(msg, e, n);
}

// RSA 解密
int decrypt(int msg, int d, int n) {
    return powerMod(msg, d, n);
}

int main() {
    // 选择两个不同的素数 p 和 q
    int p = 11;
    int q = 13;

    // 计算模数 n 和欧拉函数 phi(n)
    int n = p * q;
    int phi = (p - 1) * (q - 1);

    // 选择指数 e，确保与 phi 互质
    int e = 7;

    // 计算 d，即 e 的模反元素
    int d = modInverse(e, phi);

    // 明文
    int msg = 9;

    // 加密明文
    int encrypted_msg = encrypt(msg, e, n);
    std::cout << "Encrypted message: " << encrypted_msg << std::endl;

    // 解密密文
    int decrypted_msg = decrypt(encrypted_msg, d, n);
    std::cout << "Decrypted message: " << decrypted_msg << std::endl;

    return 0;
}
