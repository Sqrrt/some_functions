//
// Created by oleg on 15.12.2024.
//

#ifndef CRYPT_SHA256_H
#define CRYPT_SHA256_H
#include <vector>
#include <bitset>
#include <cmath>
#include <sstream>
#include <cstdint>
#include <iomanip>


constexpr size_t BLOCK_SIZE = 64;
//функции создания блоков
std::string correct_binary(const std::string &x);
std::vector<std::string> split_to_blocks(const std::string& blocks, int maxLength = 512);
std::vector<uint32_t> split_to_uint32(const std::string& binary_str);

//вспомогательные функции
uint32_t get_fractional_bits(double number);
uint32_t right_rotate(uint32_t word, int center);
uint32_t s0(uint32_t word);
uint32_t s1(uint32_t word);
uint32_t sigma0(uint32_t a);
uint32_t sigma1(uint32_t e);
uint32_t Choice(uint32_t e, uint32_t f, uint32_t g);
uint32_t Majority(uint32_t a, uint32_t b, uint32_t c);
uint32_t Temp1(uint32_t e, uint32_t f, uint32_t g, uint32_t h, uint32_t word, uint32_t m);
uint32_t Temp2(uint32_t a,uint32_t b,uint32_t c);
std::vector<uint32_t> to_blocks(std::vector<uint32_t> &blocks);
std::vector<uint32_t> mas_hashes64();
std::vector<uint32_t> get_random();
std::vector<uint32_t> compress(std::vector<uint32_t> mas_of_words,
                               std::vector<uint32_t > mas_hashes,
                               std::vector<uint32_t> random);

//sha-256!
std::string sha256(const std::string &message);

#endif //CRYPT_SHA256_H
