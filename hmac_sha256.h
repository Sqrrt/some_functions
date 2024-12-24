#ifndef CRYPT_HMAC_SHA256_H
#define CRYPT_HMAC_SHA256_H
#include <iostream>
#include <vector>
#include <sstream>
#include <cstdint>
#include <iomanip>

//функции преобразования данных
std::vector<uint8_t> xor_vectors(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b);
std::string string_to_hex(const std::string& input);
std::vector<uint8_t> hex_to_uint8(const std::string& hexStr);

//hmac_sha256
std::string hmac_sha256(const std::string& key, const std::string& message);

#endif //CRYPT_HMAC_SHA256_H
