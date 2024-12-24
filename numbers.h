

#ifndef CRYPT_NUMBERS_H
#define CRYPT_NUMBERS_H
#include <vector>


std::vector<int> sieve_eratosthenes(int n);
int gcd(int a, int b);
std::pair<int,int> evklid(int a, int b);
int mod_inv(int a, int b);

#endif //CRYPT_NUMBERS_H
