#include "Point.h"

using namespace std;


Point::Point() : x(nullptr), y(nullptr), isZero(true) {}

Point::Point(BIGNUM* x, BIGNUM* y, map<string, BIGNUM*> config) : x(BN_dup(x)), y(BN_dup(y)), config(config), isZero(false) {
    checkOnCurve();
}

Point::Point(map<string, BIGNUM*> config) : x(nullptr), y(nullptr), config(config), isZero(true) {}

Point::Point(const Point& other) : x(BN_dup(other.x)), y(BN_dup(other.y)), config(other.config), isZero(other.isZero) {}

Point::~Point() {
    if (x) BN_free(x);
    if (y) BN_free(y);
}

BIGNUM* Point::getX() const { return x; }
BIGNUM* Point::getY() const { return y; }

void Point::checkOnCurve() const {
    if (isZero) return;

    BN_CTX* ctx = BN_CTX_new();
    BIGNUM* lhs = BN_new();
    BIGNUM* rhs = BN_new();
    BIGNUM* temp = BN_new();

    BN_mod_sqr(lhs, y, config.at("p"), ctx);

    BN_mod_sqr(temp, x, config.at("p"), ctx);
    BN_mod_mul(temp, temp, x, config.at("p"), ctx);
    BN_mod_add(rhs, temp, config.at("b"), config.at("p"), ctx);
    BN_mod_mul(temp, config.at("a"), x, config.at("p"), ctx);
    BN_mod_add(rhs, rhs, temp, config.at("p"), ctx);

    if (BN_cmp(lhs, rhs) != 0) {
        BN_free(lhs);
        BN_free(rhs);
        BN_free(temp);
        BN_CTX_free(ctx);
        throw invalid_argument("Point is not on the curve");
    }

    BN_free(lhs);
    BN_free(rhs);
    BN_free(temp);
    BN_CTX_free(ctx);
}

bool Point::is_equal(const Point& other) const {
    if (isZero && other.isZero) return true;
    if (isZero || other.isZero) return false;
    return BN_cmp(x, other.x) == 0 && BN_cmp(y, other.y) == 0;
}

Point Point::add(const Point& other) const {
    if (isZero) return other;
    if (other.isZero) return *this;

    BN_CTX* ctx = BN_CTX_new();
    BIGNUM* slope = BN_new();
    BIGNUM* x3 = BN_new();
    BIGNUM* y3 = BN_new();

    if (is_equal(other)) {
        BIGNUM* num = BN_new();
        BIGNUM* den = BN_new();
        BIGNUM* three = BN_new();
        BN_set_word(three, 3);

        BN_mod_sqr(num, x, config.at("p"), ctx);
        BN_mod_mul(num, num, three, config.at("p"), ctx);
        BN_mod_add(num, num, config.at("a"), config.at("p"), ctx);

        BN_mod_lshift1(den, y, config.at("p"), ctx);
        BN_mod_inverse(den, den, config.at("p"), ctx);
        BN_mod_mul(slope, num, den, config.at("p"), ctx);

        BN_free(num);
        BN_free(den);
        BN_free(three);
    } else {
        BIGNUM* num = BN_new();
        BIGNUM* den = BN_new();

        BN_mod_sub(num, other.y, y, config.at("p"), ctx);
        BN_mod_sub(den, other.x, x, config.at("p"), ctx);
        BN_mod_inverse(den, den, config.at("p"), ctx);
        BN_mod_mul(slope, num, den, config.at("p"), ctx);

        BN_free(num);
        BN_free(den);
    }
    BN_mod_sqr(x3, slope, config.at("p"), ctx);
    BN_mod_sub(x3, x3, x, config.at("p"), ctx);
    BN_mod_sub(x3, x3, other.x, config.at("p"), ctx);

    BN_mod_sub(y3, x, x3, config.at("p"), ctx);
    BN_mod_mul(y3, y3, slope, config.at("p"), ctx);
    BN_mod_sub(y3, y3, y, config.at("p"), ctx);

    Point result(BN_dup(x3), BN_dup(y3), config);

    BN_free(slope);
    BN_free(x3);
    BN_free(y3);
    BN_CTX_free(ctx);

    return result;
}

Point Point::multiply(const BIGNUM* k) const {
    Point result(config);
    Point base(*this);
    BIGNUM* n = BN_dup(k);

    while (!BN_is_zero(n)) {
        if (BN_is_odd(n)) {
            result = result.add(base);
        }
        base = base.add(base);
        BN_rshift1(n, n);
    }

    BN_free(n);
    return result;
}

Point& Point::operator=(const Point& other) {
    if (this == &other) return *this;
    if (x) BN_free(x);
    if (y) BN_free(y);
    x = BN_dup(other.x);
    y = BN_dup(other.y);
    config = other.config;
    isZero = other.isZero;
    return *this;
}