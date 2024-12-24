#ifndef CRYPT_RSA_H
#define CRYPT_RSA_H
#include "numbers"
#include <iostream>
#include <cmath>
#include <random>
#include <openssl/bn.h>
#include <openssl/err.h>
#include <cstring>


BIGNUM* find_private_exponent(BIGNUM* e, BIGNUM* phi);
BIGNUM* generate_prime(int bits, bool safe = false);
std::pair<std::pair<BIGNUM*,BIGNUM*>, std::pair<BIGNUM*, BIGNUM*>> pair_gen();
std::string RSA_encrypt(const std::string& message, BIGNUM* e, BIGNUM* n);
std::string RSA_decrypt(const std::string& secret, BIGNUM* d, BIGNUM* n);

#endif //CRYPT_RSA_H
