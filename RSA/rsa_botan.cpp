#include <botan/auto_rng.h>
#include <botan/hex.h>
#include <botan/pem.h>
#include <botan/pkcs8.h>
#include <botan/pubkey.h>
#include <botan/rsa.h>
#include <botan/x509_key.h>
#include <iostream>
#include <vector>

int main() {
    try {
        // 初始化随机数生成器
        Botan::AutoSeeded_RNG rng;

        // 生成RSA密钥对
        Botan::RSA_PrivateKey private_key(rng, 2048);
        Botan::RSA_PublicKey public_key = private_key;

        // 获取私钥和公钥的PEM格式字符串
        std::string private_key_pem = Botan::PKCS8::PEM_encode(private_key);
        std::string public_key_pem = Botan::X509::PEM_encode(public_key);

        // 打印私钥和公钥
        std::cout << "Private Key:\n"
                  << private_key_pem << std::endl;
        std::cout << "Public Key:\n"
                  << public_key_pem << std::endl;

        // 要加密的消息
        std::string message = "Hello, Botan!";

        // 加密消息
        Botan::PK_Encryptor_EME encryptor(public_key, rng, "EME1(SHA-256)");
        std::vector<uint8_t> ciphertext = encryptor.encrypt(
                reinterpret_cast<const uint8_t *>(message.data()), message.size(), rng);

        std::cout << "Encrypted message: " << Botan::hex_encode(ciphertext) << std::endl;

        // 解密消息
        Botan::PK_Decryptor_EME decryptor(private_key, rng, "EME1(SHA-256)");
        Botan::secure_vector<uint8_t> decrypted = decryptor.decrypt(ciphertext.data(), ciphertext.size());

        std::string decrypted_message(decrypted.begin(), decrypted.end());
        std::cout << "Decrypted message: " << decrypted_message << std::endl;

    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
