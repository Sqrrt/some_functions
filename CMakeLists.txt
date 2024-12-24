cmake_minimum_required(VERSION 3.29)
project(crypt)

set(CMAKE_CXX_STANDARD 20)

add_executable(crypt main.cpp
        sha3.cpp
        sha256.cpp
        numbers.cpp
        hmac_sha256.cpp
        RSA.cpp
        texts_and_numbers.cpp
        Point.cpp
        ECDSA.cpp)

set(OPENSSL_ROOT_DIR "C:/tools/OpenSSL")
set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(OPENSSL_CRYPTO_LIBRARY "${OPENSSL_ROOT_DIR}/lib/VC/x64/MDd/libcrypto.lib")
    set(OPENSSL_SSL_LIBRARY "${OPENSSL_ROOT_DIR}/lib/VC/x64/MDd/libssl.lib")
else()
    set(OPENSSL_CRYPTO_LIBRARY "${OPENSSL_ROOT_DIR}/lib/VC/x64/MD/libcrypto.lib")
    set(OPENSSL_SSL_LIBRARY "${OPENSSL_ROOT_DIR}/lib/VC/x64/MD/libssl.lib")
endif()

target_include_directories(crypt PRIVATE ${OPENSSL_INCLUDE_DIR})
target_link_libraries(crypt ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY})