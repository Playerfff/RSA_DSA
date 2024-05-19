#include <botan/auto_rng.h>
#include <botan/dsa.h>
#include <botan/pem.h>
#include <botan/hex.h>
#include <botan/pubkey.h>
#include <botan/dl_group.h>
#include <botan/der_enc.h>
#include <botan/pubkey.h>
#include <iostream>
#include <memory>

// 生成DSA密钥对
void GenerateDSAKeys(std::unique_ptr<Botan::Private_Key>& privateKey, std::unique_ptr<Botan::Public_Key>& publicKey) {
    Botan::AutoSeeded_RNG rng;
    Botan::DL_Group group(rng, Botan::DL_Group::DSA_Kosherizer, 2048);
    privateKey = std::make_unique<Botan::DSA_PrivateKey>(rng, group);
    publicKey = std::make_unique<Botan::DSA_PublicKey>(dynamic_cast<Botan::DSA_PrivateKey&>(*privateKey));
}

// 使用私钥对消息进行签名
std::vector<uint8_t> SignMessage(const Botan::DSA_PrivateKey& privateKey, const std::string& message) {
    Botan::AutoSeeded_RNG rng;
    std::vector<uint8_t> messageVec(message.begin(), message.end());
    Botan::PK_Signer signer(privateKey, rng, "EMSA1(SHA-256)");
    return signer.sign_message(messageVec, rng);
}

// 使用公钥验证签名
bool VerifyMessage(const Botan::DSA_PublicKey& publicKey, const std::string& message, const std::vector<uint8_t>& signature) {
    std::vector<uint8_t> messageVec(message.begin(), message.end());
    Botan::PK_Verifier verifier(publicKey, "EMSA1(SHA-256)");
    return verifier.verify_message(messageVec, signature);
}

int main() {
    try {
        // 生成DSA密钥对
        std::unique_ptr<Botan::Private_Key> privateKey;
        std::unique_ptr<Botan::Public_Key> publicKey;
        GenerateDSAKeys(privateKey, publicKey);

        // 保存公钥
        std::vector<uint8_t> pubKeyDER;
        Botan::DER_Encoder derEncoder(pubKeyDER);
        publicKey->encode(derEncoder);
        std::string pubKeyPEM = Botan::PEM_Code::encode(pubKeyDER, "DSA PUBLIC KEY");
        std::cout << "Public Key: " << std::endl << pubKeyPEM << std::endl;

        // 待签名的消息
        std::string message = "This is a test message.";

        // 使用私钥对消息进行签名
        std::vector<uint8_t> signature = SignMessage(dynamic_cast<Botan::DSA_PrivateKey&>(*privateKey), message);
        std::string encodedSignature = Botan::hex_encode(signature);
        std::cout << "Signature: " << encodedSignature << std::endl;

        // 使用公钥验证签名
        bool result = VerifyMessage(dynamic_cast<Botan::DSA_PublicKey&>(*publicKey), message, signature);
        std::cout << "Signature is " << (result ? "valid" : "invalid") << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
