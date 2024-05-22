#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <tuple>

bool is_prime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

int generate_prime(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    int prime;
    do {
        prime = dis(gen);
    } while (!is_prime(prime));
    return prime;
}

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

std::tuple<int, int, int> extended_gcd(int a, int b) {
    if (b == 0) return std::make_tuple(a, 1, 0);
    auto [g, x1, y1] = extended_gcd(b, a % b);
    int x = y1;
    int y = x1 - (a / b) * y1;
    return std::make_tuple(g, x, y);
}

int mod_inverse(int e, int phi) {
    auto [g, x, y] = extended_gcd(e, phi);
    if (g != 1) throw std::runtime_error("Inverse doesn't exist");
    return (x % phi + phi) % phi;
}

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

struct RSAKeyPair {
    int e, d, n;
};

RSAKeyPair generate_rsa_keys(int bit_length) {
    int min = 1 << (bit_length / 2 - 1);
    int max = 1 << (bit_length / 2);

    int p = generate_prime(min, max);
    int q = generate_prime(min, max);
    int n = p * q;
    int phi = (p - 1) * (q - 1);

    int e = 65537; // 常用的公钥指数
    if (gcd(e, phi) != 1) throw std::runtime_error("e is not coprime with phi");

    int d = mod_inverse(e, phi);
    return {e, d, n};
}

int encrypt(int message, int e, int n) {
    return mod_pow(message, e, n);
}

int decrypt(int ciphertext, int d, int n) {
    return mod_pow(ciphertext, d, n);
}

int main() {
    int bit_length = 16; // 简化为16位，实际应用应至少使用2048位
    RSAKeyPair keys = generate_rsa_keys(bit_length);

    std::cout << "Public Key (e, n): (" << keys.e << ", " << keys.n << ")\n";
    std::cout << "Private Key (d, n): (" << keys.d << ", " << keys.n << ")\n";

    int message = 42; // 示例消息
    int encrypted_message = encrypt(message, keys.e, keys.n);
    int decrypted_message = decrypt(encrypted_message, keys.d, keys.n);

    std::cout << "Original Message: " << message << "\n";
    std::cout << "Encrypted Message: " << encrypted_message << "\n";
    std::cout << "Decrypted Message: " << decrypted_message << "\n";

    return 0;
}

