#include "RSA.h"

using namespace std;






BIGNUM* find_private_exponent(BIGNUM* e, BIGNUM* phi) {
    BN_CTX* ctx = BN_CTX_new();
    BIGNUM* one = BN_new();
    BIGNUM* d = BN_new();

    BN_one(one);

    if (!BN_mod_inverse(d, e, phi, ctx)) {
        std::cerr << "Error: Could not compute modular inverse" << std::endl;
        BN_free(d);
        d = nullptr;
    }

    BN_free(one);
    BN_CTX_free(ctx);

    return d;
}

BIGNUM* generate_prime(int bits, bool safe) {
    BIGNUM* prime = BN_new();
    BN_generate_prime_ex(prime, bits, safe, nullptr, nullptr, nullptr);
    return prime;
}

pair<pair<BIGNUM*,BIGNUM*>, pair<BIGNUM*, BIGNUM*>> pair_gen(){
    int num_bits = 512;
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *eiler = BN_new();
    BIGNUM *one = BN_new();
    BIGNUM *a_1 = BN_new();
    BIGNUM *b_1 = BN_new();
    BIGNUM *exp = BN_new();

    BIGNUM *a = generate_prime(num_bits);
    BIGNUM *b;
    do {
        b = generate_prime(num_bits, ctx);

    } while (BN_cmp(a, b) == 0);
    BIGNUM *n = BN_new();
    BN_mul(n,a,b, ctx);
    //cout<<"\n\n";
    //cout<< BN_bn2dec(a)<<" and "<<BN_bn2dec(b)<< endl;
    BN_one(one);


    BN_sub(a_1, a,one);
    BN_sub(b_1,b,one);

    BN_mul(eiler,a_1,b_1, ctx);
    //cout<<"\n\n";
    //cout<< BN_bn2dec(eiler)<< endl;
    do {
        BN_rand_range(exp, eiler);
    } while (BN_cmp(exp, one) <= 0 || !BN_gcd(one, exp, eiler, ctx) || BN_cmp(one, BN_value_one()) != 0);
    BIGNUM* d = find_private_exponent(exp,eiler);
    pair<BIGNUM*,BIGNUM*> p1 = {exp,n};
    pair<BIGNUM*, BIGNUM*> p2 = {d,n};
    pair<pair<BIGNUM*,BIGNUM*>, pair<BIGNUM*, BIGNUM*>> pres = {p1,p2};

    BN_free(a_1);
    BN_free(b_1);
    BN_free(eiler);
    BN_free(one);
    BN_free(exp);
    BN_CTX_free(ctx);

    return pres;
}

string RSA_encrypt(const string& message, BIGNUM* e, BIGNUM* n){
    BN_CTX* ctx = BN_CTX_new();
    const char* s1 = message.c_str();
    BIGNUM* mes = BN_new();
    BN_dec2bn(&mes,s1);
    BIGNUM* c = BN_new();
    BN_mod_exp(c,mes,e,n, ctx);

    BN_free(mes);
    BN_CTX_free(ctx);
    return BN_bn2dec(c);
}

string RSA_decrypt(const string& secret, BIGNUM* d, BIGNUM* n){
    BN_CTX* ctx = BN_CTX_new();
    const char* s1 = secret.c_str();
    BIGNUM* s = BN_new();
    BN_dec2bn(&s,s1);
    BIGNUM* c = BN_new();
    BN_mod_exp(c,s,d,n, ctx);

    BN_free(s);
    BN_CTX_free(ctx);
    return BN_bn2dec(c);
}