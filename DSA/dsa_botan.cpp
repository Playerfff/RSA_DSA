#include <botan/auto_rng.h>
#include <botan/dl_group.h>
#include <botan/dsa.h>
#include <botan/hash.h>
#include <botan/hex.h>
#include <botan/pem.h>
#include <botan/pkcs8.h>
#include <botan/pubkey.h>
#include <botan/x509_key.h>
#include <iostream>
#include <vector>

void handle_errors(const std::string &msg) {
    std::cerr << "Error: " << msg << std::endl;
    exit(1);
}

int main() {
    try {
        // 初始化随机数生成器
        Botan::AutoSeeded_RNG rng;

        // 生成DSA参数
        Botan::DL_Group group("dsa/botan/2048");

        // 生成DSA密钥对
        Botan::DSA_PrivateKey private_key(rng, group);
        Botan::DSA_PublicKey public_key = private_key;

        // 获取私钥和公钥的PEM格式字符串
        std::string private_key_pem = Botan::PKCS8::PEM_encode(private_key);
        std::string public_key_pem = Botan::X509::PEM_encode(public_key);

        // 打印私钥和公钥
        std::cout << "Private Key:\n" << private_key_pem << std::endl;
        std::cout << "Public Key:\n" << public_key_pem << std::endl;

        // 要签名的消息
        std::string message = "Hello, Botan DSA!";

        // 计算消息的哈希值
        const auto hash = Botan::HashFunction::create_or_throw("SHA-256");
        auto message_hash = hash->process(message);

        // 签名消息
        Botan::PK_Signer signer(private_key, rng, "EMSA1(SHA-256)");
        std::vector<uint8_t> signature = signer.sign_message(message_hash, rng);

        std::cout << "Signature: " << Botan::hex_encode(signature) << std::endl;

        // 验证签名
        Botan::PK_Verifier verifier(public_key, "EMSA1(SHA-256)");
        bool valid = verifier.verify_message(message_hash, signature);

        std::cout << "Signature is " << (valid ? "valid" : "invalid") << std::endl;

    } catch (std::exception &e) {
        handle_errors(e.what());
    }

    return 0;
}
