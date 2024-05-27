#include <iostream>

#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>

using namespace std;
using namespace CryptoPP;


// 生成RSA密钥对
void GenerateRSAKeyPair(RSA::PrivateKey &privateKey, RSA::PublicKey &publicKey) {
    AutoSeededRandomPool rng;// 创建一个自动播种的随机数生成器，用于生成密钥时所需的随机性

    InvertibleRSAFunction parameters;               // 定义RSA函数参数结构，用于存储生成的密钥对参数
    parameters.GenerateRandomWithKeySize(rng, 2048);// 使用随机数生成器rng生成一个2048位长度的RSA密钥对参数

    // 分别从生成的参数中创建私钥和公钥对象
    privateKey = RSA::PrivateKey(parameters);
    publicKey = RSA::PublicKey(parameters);
}

// RSA加密
std::string RSAEncrypt(const RSA::PublicKey &publicKey, const std::string &plainText) {
    AutoSeededRandomPool rng;
    RSAES_OAEP_SHA_Encryptor encryptor(publicKey);// 创建一个基于RSAES-OAEP-SHA算法的加密器

    std::string cipherText;

    StringSource(plainText, true,
                 new PK_EncryptorFilter(rng, encryptor,
                                        new StringSink(cipherText))// 构建一个过滤器链，它首先使用随机数生成器rng，然后通过加密器encryptor进行加密，最后将加密结果存入cipherText字符串中。
    );                                                             // 创建一个字符串源，用于读取明文字符串，并在读取完毕后自动关闭

    return cipherText;
}

// RSA解密
std::string RSADecrypt(const RSA::PrivateKey &privateKey, const std::string &cipherText) {
    AutoSeededRandomPool rng;
    RSAES_OAEP_SHA_Decryptor decryptor(privateKey);// 创建一个基于RSAES-OAEP-SHA算法的解密器，与加密器对应。

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
