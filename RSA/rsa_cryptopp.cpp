#include <iostream>

#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>

using namespace std;
using namespace CryptoPP;


// 生成RSA密钥对
void GenerateRSAKeyPair(RSA::PrivateKey &privateKey, RSA::PublicKey &publicKey) {
    AutoSeededRandomPool rng;

    InvertibleRSAFunction parameters;
    parameters.GenerateRandomWithKeySize(rng, 2048);

    privateKey = RSA::PrivateKey(parameters);
    publicKey = RSA::PublicKey(parameters);
}

// RSA加密
std::string RSAEncrypt(const RSA::PublicKey &publicKey, const std::string &plainText) {
    AutoSeededRandomPool rng;
    RSAES_OAEP_SHA_Encryptor encryptor(publicKey);

    std::string cipherText;

    StringSource(plainText, true,
                 new PK_EncryptorFilter(rng, encryptor,
                                        new StringSink(cipherText)));

    return cipherText;
}

// RSA解密
std::string RSADecrypt(const RSA::PrivateKey &privateKey, const std::string &cipherText) {
    AutoSeededRandomPool rng;
    RSAES_OAEP_SHA_Decryptor decryptor(privateKey);

    std::string recoveredText;

    StringSource(cipherText, true,
                 new PK_DecryptorFilter(rng, decryptor,
                                        new StringSink(recoveredText)));

    return recoveredText;
}


int main(int argc, char *argv[]) {
    try {
        RSA::PrivateKey privateKey;
        RSA::PublicKey publicKey;

        // 生成RSA密钥对
        GenerateRSAKeyPair(privateKey, publicKey);

        // 待加密的文本
        std::string plainText = "Hello, world !";

        std::cout << "plainText: " << plainText << std::endl;

        // RSA加密
        std::string cipherText = RSAEncrypt(publicKey, plainText);
        std::cout << "Cipher Text: " << cipherText << std::endl;

        // RSA解密
        std::string recoveredText = RSADecrypt(privateKey, cipherText);
        std::cout << "Recovered Text: " << recoveredText << std::endl;
    } catch (CryptoPP::Exception &e) {
        std::cerr << "Crypto++ Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
