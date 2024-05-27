#include <openssl/dsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <iostream>
#include <string>
#include <vector>

// 该函数用于处理OpenSSL错误，将错误信息打印到标准错误输出，并终止程序。
void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

DSA* generateDSAKeyPair() {
    // 初始化DSA结构体
    DSA* dsa = DSA_new();
    // 生成DSA密钥对所需的参数
    if (!DSA_generate_parameters_ex(dsa, 2048, NULL, 0, NULL, NULL, NULL)) {
        handleErrors();
    }
    // 基于之前生成的参数来实际生成DSA的密钥对（包括私钥和公钥）
    if (!DSA_generate_key(dsa)) {
        handleErrors();
    }
    return dsa;
}

std::vector<unsigned char> dsaSign(DSA* dsa, const std::string& message) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    // 计算需要签名的消息的SHA-256哈希值
    SHA256((unsigned char*)message.c_str(), message.size(), hash);

    std::vector<unsigned char> sig(DSA_size(dsa));
    unsigned int sig_len;

    // 对哈希值进行签名，并将签名结果存储在sig向量中
    if (DSA_sign(0, hash, SHA256_DIGEST_LENGTH, sig.data(), &sig_len, dsa) == 0) {
        handleErrors();
    }

    // 调整签名向量的大小以匹配实际签名长度后
    sig.resize(sig_len);
    return sig;
}

bool dsaVerify(DSA* dsa, const std::string& message, const std::vector<unsigned char>& sig) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    // 计算需要签名的消息的SHA-256哈希值
    SHA256((unsigned char*)message.c_str(), message.size(), hash);

    return DSA_verify(0, hash, SHA256_DIGEST_LENGTH, sig.data(), sig.size(), dsa) == 1;
}

int main() {
    // Initialize OpenSSL
    OpenSSL_add_all_algorithms();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();

    // Generate DSA key pair
    DSA* dsa = generateDSAKeyPair();
    if (!dsa) {
        std::cerr << "Key pair generation failed" << std::endl;
        return 1;
    }

    // 创建了两个流（Basic I/O）用于存放公钥和私钥的PEM编码形式
    BIO* bp_public = BIO_new(BIO_s_mem());
    BIO* bp_private = BIO_new(BIO_s_mem());

    // 将RSA结构体中的公钥和私钥写入到之前创建的流中
    PEM_write_bio_DSA_PUBKEY(bp_public, dsa);
    PEM_write_bio_DSAPrivateKey(bp_private, dsa, NULL, NULL, 0, NULL, NULL);

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

    // 签名
    std::string message = "Hello, OpenSSL DSA!";
    std::vector<unsigned char> signature = dsaSign(dsa, message);
    std::cout << "Signature: ";
    for (unsigned char c : signature) {
        // 将字符c转换成其对应的十六进制数值
        printf("%02x", c);
    }
    std::cout << std::endl;

    // 验证
    bool is_valid = dsaVerify(dsa, message, signature);
    std::cout << "Signature is " << (is_valid ? "valid" : "invalid") << std::endl;

    // Free DSA key
    DSA_free(dsa);

    // Clean up OpenSSL
    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();

    return 0;
}
