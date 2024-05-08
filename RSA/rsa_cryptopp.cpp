#include <iostream>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <cryptopp/base64.h>
#include <cryptopp/rsa.h>

// 生成RSA密钥对并保存到文件
void GenerateAndSaveKeys(const std::string& publicKeyFileName, const std::string& privateKeyFileName) {
    CryptoPP::AutoSeededRandomPool rng;

    CryptoPP::InvertibleRSAFunction params;
    params.GenerateRandomWithKeySize(rng, 2048);

    CryptoPP::RSA::PublicKey publicKey(params);
    CryptoPP::RSA::PrivateKey privateKey(params);

    CryptoPP::FileSink pubSink(publicKeyFileName.c_str());
    publicKey.Save(pubSink);
    pubSink.MessageEnd();

    CryptoPP::FileSink privSink(privateKeyFileName.c_str(), true);
    privateKey.Save(privSink);
    privSink.MessageEnd();
}

// 从文件加载RSA密钥
CryptoPP::RSA::PublicKey LoadPublicKey(const std::string& fileName) {
    CryptoPP::ByteQueue queue;
    CryptoPP::FileSource(fileName.c_str(), true, new CryptoPP::Redirector(queue));
    CryptoPP::RSA::PublicKey publicKey;
    publicKey.Load(queue);
    return publicKey;
}

CryptoPP::RSA::PrivateKey LoadPrivateKey(const std::string& fileName) {
    CryptoPP::ByteQueue queue;
    CryptoPP::FileSource(fileName.c_str(), true, new CryptoPP::Redirector(queue));
    CryptoPP::RSA::PrivateKey privateKey;
    privateKey.Load(queue);
    return privateKey;
}

// RSA加密
std::string Encrypt(const std::string& plaintext, const CryptoPP::RSA::PublicKey& publicKey) {
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::RSAES_OAEP_SHA_Encryptor encryptor(publicKey);

    std::string ciphertext;
    CryptoPP::StringSource(plaintext, true,
                           new CryptoPP::PK_EncryptorFilter(rng, encryptor,
                                                            new CryptoPP::StringSink(ciphertext)
                                                                    ) // PK_EncryptorFilter
    ); // StringSource
    return ciphertext;
}

// RSA解密
std::string Decrypt(const std::string& ciphertext, const CryptoPP::RSA::PrivateKey& privateKey) {
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::RSAES_OAEP_SHA_Decryptor decryptor(privateKey);

    std::string recoveredText;
    CryptoPP::StringSource(ciphertext, true,
                           new CryptoPP::PK_DecryptorFilter(rng, decryptor,
                                                            new CryptoPP::StringSink(recoveredText)
                                                                    ) // PK_DecryptorFilter
    ); // StringSource
    return recoveredText;
}

int main() {
    const std::string publicKeyFileName = "rsa_public_key.pem";
    const std::string privateKeyFileName = "rsa_private_key.pem";

    // 生成并保存密钥对
    GenerateAndSaveKeys(publicKeyFileName, privateKeyFileName);

    // 加载公钥和私钥
    CryptoPP::RSA::PublicKey publicKey = LoadPublicKey(publicKeyFileName);
    CryptoPP::RSA::PrivateKey privateKey = LoadPrivateKey(privateKeyFileName);

    // 待加密的明文
    std::string message = "Hello, this is a secret message!";
    std::cout << "Original Message: " << message << std::endl;

    // 加密
    std::string encrypted = Encrypt(message, publicKey);
    std::cout << "Encrypted Message: " << encrypted << std::endl;

    // 解密
    std::string decrypted = Decrypt(encrypted, privateKey);
    std::cout << "Decrypted Message: " << decrypted << std::endl;

    return 0;
}