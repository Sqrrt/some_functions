#include <iostream>
#include <bitset>
#include "sha256.h"
#include "hmac_sha256.h"
#include "RSA.h"
#include <openssl/bn.h>
#include "Point.h"
#include "ECDSA.h"
#include "sha3.h"
using namespace std;

int main() {
    string message = "hallo, world";
    string other_message = "hello, world";
    //string message3 = "hello, world";
    cout<< "Hash functions:"<<endl;
    string hash = sha256(message);
    string hash2 = sha3_256(message);
    string hash3 = sha256(other_message);
    string hash4 = sha3_256(other_message);
    cout << "SHA-256 (hallo, world) hash: " << hash <<" "<< hash.size()<< endl;
    cout << "SHA-256 (hello, world) hash: " << hash3 <<" "<< hash3.size()<< endl;
    cout << "SHA3-256 (hallo, world) hash: " << hash2 <<" "<< hash2.size()<< endl;
    cout << "SHA3-256 (hello, world) hash: " << hash4 <<" "<< hash4.size()<< endl;
    cout << "\n";
    cout << "HMAC results:"<< endl;
    std::string key = "wow, what the key, it's incredible";
    std::string result = hmac_sha256(key, message);
    cout<< "HMAC value (hello, world) : "<< result<<" "<< result.size() << endl;
    std::string result2 = hmac_sha256(key, message);
    cout<< "HMAC value (hello, world) : "<< result2<<" "<< result2.size() << endl;
    std::string result3 = hmac_sha256(key, other_message);
    cout<< "HMAC value (hallo, world) : "<< result3<<" "<< result3.size() << endl;
    cout << "\n";
    cout << "RSA results:"<< endl;
    pair<pair<BIGNUM*, BIGNUM*>, pair<BIGNUM*, BIGNUM*>> p = pair_gen();
    string message2 = "112345765";
    BIGNUM* e = BN_new();
    BIGNUM* d = BN_new();
    BIGNUM* n = BN_new();
    BN_copy(e, p.first.first);
    BN_copy(d, p.second.first);
    BN_copy(n, p.first.second);
    string secret = RSA_encrypt(message2,e,n);
    cout<<"secret is: "<< secret<< endl;
    string message2_true = RSA_decrypt(secret, d, n);
    cout<<"message is: "<< message2_true<<"\n";
    BN_free(e);
    BN_free(d);
    BN_free(n);
    cout<<"\n";

    cout<< "ECDSA results: "<< endl;
    map<string, BIGNUM*> curve_config;
    BIGNUM* a = BN_new();
    BN_dec2bn(&a, "0");
    BIGNUM* b = BN_new();
    BN_dec2bn(&b, "7");
    BIGNUM* p1 = BN_new();
    BN_dec2bn(&p1, "115792089237316195423570985008687907853269984665640564039457584007908834671663");
    curve_config["a"] = a;
    curve_config["b"] = b;
    curve_config["p"] = p1;

    BIGNUM* n1 = BN_new();
    BN_dec2bn(&n1, "115792089237316195423570985008687907852837564279074904382605163141518161494337");

    BIGNUM* x = BN_new();
    BIGNUM* y = BN_new();
    BN_dec2bn(&x, "55066263022277343669578718895168534326250603453777594175500187360389116729240");
    BN_dec2bn(&y, "32670510020758816978083085130507043184471273380659243275938904335757337482424");

    Point point(x,y,curve_config);

    BIGNUM* private_key = BN_new();
    BN_dec2bn(&private_key, "88005553535");
    BIGNUM* private_key_no = BN_new();
    BN_dec2bn(&private_key_no, "88005553555");
    BIGNUM* message_el = BN_new();
    BN_dec2bn(&message_el, "11223355");
    pair<BIGNUM*, BIGNUM*> enc = sign_message(message_el, private_key, point, n1);
    Point public_key_no = point.multiply(private_key_no);
    Point public_key = point.multiply(private_key);

    cout<<"\n Verification 1:  "<< verify_signature(enc,message_el,public_key,n1,point) << endl;
    cout<<"\n Verification 2:  "<< verify_signature(enc,message_el,public_key_no,n1,point) << endl;




    return 0;
}