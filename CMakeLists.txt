cmake_minimum_required(VERSION 3.28)
project(RSA_DSA)

set(CMAKE_CXX_STANDARD 17)

# 查找OpenSSL库
find_package(OpenSSL REQUIRED)

# 查找Crypto++库
find_package(Git REQUIRED)

if (NOT EXISTS "${CMAKE_SOURCE_DIR}/cryptopp")
    execute_process(
            COMMAND "${GIT_EXECUTABLE}" clone https://github.com/weidai11/cryptopp.git
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
endif()

set(CMAKE_SKIP_INSTALL_RULES TRUE)


# 添加可执行文件
add_executable(rsa_openssl RSA/rsa_openssl.cpp)

# 链接到OpenSSL库
target_link_libraries(rsa_openssl PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# 添加可执行文件
add_executable(rsa_cryptopp RSA/rsa_cryptopp.cpp)

# 链接Crypto++库
target_link_libraries(rsa_cryptopp PRIVATE crypto++)