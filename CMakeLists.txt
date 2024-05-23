cmake_minimum_required(VERSION 3.2)
project(RSA_DSA)

set(CMAKE_CXX_STANDARD 20)

if(ENABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")
endif()

# 查找OpenSSL库
find_package(OpenSSL REQUIRED)

# 查找Crypto++库
find_package(cryptopp CONFIG REQUIRED)

find_package(botan CONFIG REQUIRED)

add_executable(rsa_openssl RSA/rsa_openssl.cpp)
target_link_libraries(rsa_openssl PRIVATE OpenSSL::SSL OpenSSL::Crypto)

add_executable(rsa_cryptopp RSA/rsa_cryptopp.cpp)
target_link_libraries(rsa_cryptopp PRIVATE cryptopp::cryptopp)

add_executable(rsa_botan RSA/rsa_botan.cpp
        RSA/openssl_data_structure.h)
target_link_libraries(rsa_botan PRIVATE Botan::Botan-static)

add_executable(dsa_openssl DSA/dsa_openssl.cpp)
target_link_libraries(dsa_openssl PRIVATE OpenSSL::SSL OpenSSL::Crypto)

add_executable(dsa_cryptopp DSA/dsa_cryptopp.cpp)
target_link_libraries(dsa_cryptopp PRIVATE cryptopp::cryptopp)

add_executable(dsa_botan DSA/dsa_botan.cpp)
target_link_libraries(dsa_botan PRIVATE Botan::Botan-static)

add_executable(homemade_rsa homemade/rsa.cpp homemade/BigInt.cc)
add_executable(homemade_dsa homemade/dsa.cpp homemade/BigInt.cc)
