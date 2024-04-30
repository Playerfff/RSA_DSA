cmake_minimum_required(VERSION 3.28)
project(RSA_DSA)

set(CMAKE_CXX_STANDARD 20)

add_executable(rsa_openssl RSA/rsa_openssl.cpp)
