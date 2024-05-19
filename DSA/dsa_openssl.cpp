#include <openssl/dsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <iostream>
#include <fstream>
#include <vector>

// Helper function to read file into a vector
std::vector<unsigned char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    return {(std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()};
}

int main() {
    // Generate DSA key pair
    DSA* dsa = DSA_new();
    if (dsa == nullptr) {
        std::cerr << "DSA_new failed." << std::endl;
        return 1;
    }

    if (DSA_generate_parameters_ex(dsa, 1024, nullptr, 0, nullptr, nullptr, nullptr) != 1) {
        std::cerr << "DSA_generate_parameters_ex failed." << std::endl;
        return 1;
    }

    if (DSA_generate_key(dsa) != 1) {
        std::cerr << "DSA_generate_key failed." << std::endl;
        return 1;
    }

    // Save private key
    FILE* privKeyFile = fopen("dsa_priv.pem", "w");
    if (PEM_write_DSAPrivateKey(privKeyFile, dsa, nullptr, nullptr, 0, nullptr, nullptr) != 1) {
        std::cerr << "PEM_write_DSAPrivateKey failed." << std::endl;
        return 1;
    }
    fclose(privKeyFile);

    // Save public key
    FILE* pubKeyFile = fopen("dsa_pub.pem", "w");
    if (PEM_write_DSA_PUBKEY(pubKeyFile, dsa) != 1) {
        std::cerr << "PEM_write_DSA_PUBKEY failed." << std::endl;
        return 1;
    }
    fclose(pubKeyFile);

    // Read the message to be signed
    std::string message = "This is a test message.";
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)message.c_str(), message.length(), hash);

    // Sign the message
    unsigned int sigLen;
    std::vector<unsigned char> sig(DSA_size(dsa));
    if (DSA_sign(0, hash, SHA256_DIGEST_LENGTH, sig.data(), &sigLen, dsa) != 1) {
        std::cerr << "DSA_sign failed." << std::endl;
        return 1;
    }
    sig.resize(sigLen);

    // Verify the signature
    int verify = DSA_verify(0, hash, SHA256_DIGEST_LENGTH, sig.data(), sigLen, dsa);
    if (verify != 1) {
        std::cerr << "Signature verification failed." << std::endl;
        return 1;
    }

    std::cout << "Signature is valid." << std::endl;

    DSA_free(dsa);
    return 0;
}
