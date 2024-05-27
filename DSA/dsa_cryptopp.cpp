#include <cryptopp/dsa.h>
#include <cryptopp/files.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <iostream>

using namespace CryptoPP;

void GenerateDSAKeys(DSA::PrivateKey &privateKey, DSA::PublicKey &publicKey) {
    AutoSeededRandomPool rng;// 随机数生成器

    // Generate DSA parameters
    privateKey.GenerateRandomWithKeySize(rng, 1024);// 生成一个具有1024位长度的随机DSA私钥
    privateKey.MakePublicKey(publicKey);            // 从私钥派生出对应的公钥
}

std::string SignMessage(const DSA::PrivateKey &privateKey, const std::string &message) {
    AutoSeededRandomPool rng;
    DSA::Signer signer(privateKey);// 使用私钥创建一个DSA签名器

    // 构建一个过滤器链，读取消息字符串，通过签名器对其进行签名，并将签名结果存储在signature变量中
    std::string signature;
    StringSource ss1(message, true,
                     new SignerFilter(rng, signer,
                                      new StringSink(signature))// SignerFilter
    );                                                          // StringSource

    return signature;
}

bool VerifyMessage(const DSA::PublicKey &publicKey, const std::string &message, const std::string &signature) {
    // 使用公钥创建一个DSA验证器
    DSA::Verifier verifier(publicKey);

    // 构建一个过滤器链，将签名和原始消息拼接后读入，通过验证器检查签名的有效性。结果存储在一个布尔型变量result中，指示签名是否有效。
    bool result = false;
    StringSource ss2(signature + message, true,
                     new SignatureVerificationFilter(
                             verifier,
                             new ArraySink((byte *) &result, sizeof(result)))// SignatureVerificationFilter
    );                                                                       // StringSource

    return result;
}

int main() {
    DSA::PrivateKey privateKey;
    DSA::PublicKey publicKey;

    // 生成DSA keys
    GenerateDSAKeys(privateKey, publicKey);

    std::string pubKey;
    HexEncoder encoder(new StringSink(pubKey));
    publicKey.DEREncode(encoder);
    encoder.MessageEnd();
    std::cout << "Public Key: " << pubKey << std::endl;

    std::string message = "This is a test message.";

    // 签名消息
    std::string signature = SignMessage(privateKey, message);
    std::string encodedSignature;
    StringSource(signature, true, new HexEncoder(new StringSink(encodedSignature)));
    std::cout << "Signature: " << encodedSignature << std::endl;

    // 验证签名
    bool result = VerifyMessage(publicKey, message, signature);
    std::cout << "Signature is " << (result ? "valid" : "invalid") << std::endl;

    return 0;
}
