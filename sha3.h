#ifndef CRYPT_SHA3_CPP_H
#define CRYPT_SHA3_CPP_H
#include <vector>
#include <string>
#include <cmath>


std::vector<std::vector<std::vector<int>>> theta(std::vector<std::vector<std::vector<int>>> A, int w = 64);
std::vector<std::vector<std::vector<int>>> rho(std::vector<std::vector<std::vector<int>>> A, int w = 64);
std::vector<std::vector<std::vector<int>>> pi(std::vector<std::vector<std::vector<int>>> A, int w = 64);
std::vector<std::vector<std::vector<int>>> chi(std::vector<std::vector<std::vector<int>>> A, int w = 64);

std::vector<std::vector<std::vector<int>>> yota(std::vector<std::vector<std::vector<int>>> A,
                                                int round_index, int l = 6, int w = 64);

std::vector<std::vector<std::vector<int>>> to_arr(const std::string& A, int w = 64);

std::vector<std::vector<std::vector<int>>> round(std::vector<std::vector<std::vector<int>>> A, int i_r);
std::string to_str(std::vector<std::vector<std::vector<int>>> A, int w = 64);

std::string trunc(const std::string& S, size_t d);
std::string pad10_1(int x, int m);
std::string keccak_p(const std::string& S);

std::string string_to_binary_string(const std::string& input);

std::string sponge(std::string (*f)(const std::string&), std::string (*pad)(int, int), int r, const std::string& N, size_t d);
std::string sha3_256(const std::string& message);


#endif //CRYPT_SHA3_CPP_H
