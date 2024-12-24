#ifndef CRYPT_ECDSA_H
#define CRYPT_ECDSA_H
#include <openssl/bn.h>
#include <map>
#include "Point.h"

/*
struct curve_and_point {
    std::map<std::string, BIGNUM*> config;
    BIGNUM* n = nullptr;
    Point p_c;
};
static curve_and_point secp256k1_curve;

void setStaticStruct() {
    std::map<std::string, BIGNUM*> config;
    BIGNUM* a = BN_new();
    BN_dec2bn(&a, "0");
    BIGNUM* b = BN_new();
    BN_dec2bn(&b, "7");
    BIGNUM* p = BN_new();
    BN_dec2bn(&p, "115792089237316195423570985008687907853269984665640564039457584007908834671663");
    config["a"] = a;
    config["b"] = b;
    config["p"] = p;
    BIGNUM* n1 = BN_new();
    BN_dec2bn(&n1, "115792089237316195423570985008687907852837564279074904382605163141518161494337");
    secp256k1_curve.n = n1;
    secp256k1_curve.config = config;

    BIGNUM* x = BN_new();
    BIGNUM* y = BN_new();
    BN_dec2bn(&x, "55066263022277343669578718895168534326250603453777594175500187360389116729240");
    BN_dec2bn(&y, "32670510020758816978083085130507043184471273380659243275938904335757337482424");
    Point p1(x,y,config);
    secp256k1_curve.p_c = p1;
}
*/
std::pair<BIGNUM*, BIGNUM*> sign_message(BIGNUM* message, BIGNUM* private_key, Point p, BIGNUM* n);
bool verify_signature(std::pair<BIGNUM*,BIGNUM*> signature, BIGNUM* message, Point public_key, BIGNUM* n, Point point);

#endif //CRYPT_ECDSA_H
