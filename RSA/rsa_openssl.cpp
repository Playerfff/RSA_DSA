#include <cstring>
#include <iostream>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

void printErrorAndExit(const char *msg) {
    ERR_print_errors_fp(stderr);
    std::cerr << msg << std::endl;
    exit(EXIT_FAILURE);
}

int main() {
    // 初始化 OpenSSL 库
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    // 生成RSA密钥对
    RSA *rsa = RSA_new();
    if (rsa == NULL) printErrorAndExit("Failed to create RSA object");

    // 生成密钥的参数，例如位数等可以根据需求调整
    BIGNUM *bn = BN_new();
    if (bn == NULL) printErrorAndExit("Failed to create BIGNUM object");
    if (!BN_set_word(bn, RSA_F4)) printErrorAndExit("Failed to set public exponent");

    // 生成RSA密钥对
    if (RSA_generate_key_ex(rsa, 2048, bn, NULL) != 1)
        printErrorAndExit("Failed to generate RSA key pair");

    // 公钥加密
    const char *plaintext = "Hello, OpenSSL RSA!";
    unsigned char encrypted[RSA_size(rsa)];
    int encrypted_len = RSA_public_encrypt(strlen(plaintext) + 1, reinterpret_cast<const unsigned char *>(plaintext), encrypted, rsa, RSA_PKCS1_PADDING);
    if (encrypted_len == -1) printErrorAndExit("Encryption failed");

    // 私钥解密
    unsigned char decrypted[encrypted_len];
    int decrypted_len = RSA_private_decrypt(encrypted_len, encrypted, decrypted, rsa, RSA_PKCS1_PADDING);
    if (decrypted_len == -1) printErrorAndExit("Decryption failed");

    decrypted[decrypted_len] = '\0';// 添加结束符
    std::cout << "Original message: " << plaintext << std::endl;
    std::cout << "Encrypted message: ";
    for (int i = 0; i < encrypted_len; ++i) std::cout << std::hex << static_cast<int>(encrypted[i]);
    std::cout << std::endl;
    std::cout << "Decrypted message: " << reinterpret_cast<char *>(decrypted) << std::endl;

    // 清理
    RSA_free(rsa);
    BN_free(bn);
    EVP_cleanup();
    CRYPTO_cleanup_all_ex_data();

    return 0;
}
