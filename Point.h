//
// Created by oleg on 18.12.2024.
//

#ifndef CRYPT_POINT_H
#define CRYPT_POINT_H
#include "numbers"
#include <iostream>
#include <cmath>
#include <random>
#include <openssl/bn.h>
#include <openssl/err.h>
#include <cstring>
#include <map>
class Point{
private:
    BIGNUM* x;
    BIGNUM* y;
    std::map<std::string, BIGNUM*> config;
    bool isZero;

public:

    Point();
    Point(std::map<std::string, BIGNUM*> conf);
    Point(BIGNUM* x, BIGNUM* y, std::map<std::string, BIGNUM*> conf);
    Point(const Point& other);

    ~Point();

    void checkOnCurve() const;
    BIGNUM* getX() const;
    BIGNUM* getY() const;

    bool is_equal(const Point& p2) const;
    Point add(const Point& p2) const;
    Point multiply(const BIGNUM* times) const;

    Point& operator=(const Point& other);
};
#endif //CRYPT_POINT_H
