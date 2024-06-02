#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <iostream>
#include <string>

// 该函数用于处理OpenSSL错误，将错误信息打印到标准错误输出，并终止程序。
void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

// 生成键值对
RSA* generateKeyPair() {
    int bits = 2048; // 2048位的RSA密钥对
    unsigned long e = RSA_F4;
    BIGNUM* bne = BN_new(); // 创建一个新的BIGNUM对象
    if (!BN_set_word(bne, e)) {
        handleErrors();
    }

    // 使用RSA_new创建一个新的RSA对象，并使用RSA_generate_key_ex生成密钥对
    RSA* rsa = RSA_new();
    if (!RSA_generate_key_ex(rsa, bits, bne, NULL)) {
        handleErrors();
    }
    // 释放BIGNUM对象，RSA对象ownership转移
    BN_free(bne);
    return rsa;
}

std::string rsaEncrypt(RSA* rsa, const std::string& plaintext) {
    std::string ciphertext(RSA_size(rsa), '\0'); // 生成加密后的字符串存储位置
    // 使用公钥加密, 选择不同的padding模式不仅可以对齐待加密的字符串, 还可以用作混淆
    int len = RSA_public_encrypt(plaintext.size(), (const unsigned char*)plaintext.c_str(),
                                 (unsigned char*)ciphertext.c_str(), rsa, RSA_PKCS1_OAEP_PADDING);
    if (len == -1) {
        handleErrors();
    }
    return ciphertext;
}

std::string rsaDecrypt(RSA* rsa, const std::string& ciphertext) {
    std::string plaintext(RSA_size(rsa), '\0');
    // 私钥解密
    int len = RSA_private_decrypt(ciphertext.size(), (const unsigned char*)ciphertext.c_str(),
                                  (unsigned char*)plaintext.c_str(), rsa, RSA_PKCS1_OAEP_PADDING);
    if (len == -1) {
        handleErrors();
    }
    plaintext.resize(len);
    return plaintext;
}

int main() {
    // Initialize OpenSSL
    OpenSSL_add_all_algorithms();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();

    // Generate RSA key pair
    RSA* rsa = generateKeyPair();
    if (!rsa) {
        std::cerr << "Key pair generation failed" << std::endl;
        return 1;
    }

    // 创建了两个流（Basic I/O）用于存放公钥和私钥的PEM编码形式
    BIO* bp_public = BIO_new(BIO_s_mem());
    BIO* bp_private = BIO_new(BIO_s_mem());

    // 将RSA结构体中的公钥和私钥写入到之前创建的流中
    PEM_write_bio_RSAPublicKey(bp_public, rsa);
    PEM_write_bio_RSAPrivateKey(bp_private, rsa, NULL, NULL, 0, NULL, NULL);

    // 获取生物流中当前待读取数据的长度
    size_t pub_len = BIO_pending(bp_public);
    size_t priv_len = BIO_pending(bp_private);

    // 分配足够的内存来存储公钥和私钥，并使用BIO_read读取流中的数据到分配的内存中
    char* pub_key = (char*)malloc(pub_len + 1);
    char* priv_key = (char*)malloc(priv_len + 1);

    BIO_read(bp_public, pub_key, pub_len);
    BIO_read(bp_private, priv_key, priv_len);

    pub_key[pub_len] = '\0';
    priv_key[priv_len] = '\0';

    std::cout << "Public Key: " << std::endl << pub_key << std::endl;
    std::cout << "Private Key: " << std::endl << priv_key << std::endl;

    // Clean up
    BIO_free_all(bp_public);
    BIO_free_all(bp_private);
    free(pub_key);
    free(priv_key);

    // Encrypt message
    std::string plaintext = "Hello, OpenSSL RSA!";
    std::string ciphertext = rsaEncrypt(rsa, plaintext);
    std::cout << "Encrypted: " << ciphertext << std::endl;

    // Decrypt message
    std::string decryptedtext = rsaDecrypt(rsa, ciphertext);
    std::cout << "Decrypted: " << decryptedtext << std::endl;

    // Free RSA key
    RSA_free(rsa);

    // Clean up OpenSSL
    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();

    return 0;
}
