#include "numbers.h"

using namespace std;
vector<int> sieve_eratosthenes(int n) {
    vector<bool> is_prime(n + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            for (int i = p * p; i <= n; i += p) {
                is_prime[i] = false;
            }
        }
    }
    vector<int> primes;
    for (int i = 2; i <= n; ++i) {
        if (is_prime[i]) {
            primes.push_back(i);
        }
    }
    return primes;
}

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

pair<int,int> evklid(int a, int b){
    int q,r;
    int x1 = 0, x2 = 1;
    int y1 = 1, y2 = 0;
    while(b > 0){
        q = a/b;
        r = a - q*b;
        int x = x2 - q*x1;
        int y = y2 - q*y1;
        a = b;
        b = r;
        x2 = x1;
        x1 = x;
        y2 = y1;
        y1 = y;
    }
    pair<int, int> p = {x2, y2};
    return p;
}

int mod_inv(int a, int b){
    pair<int,int> p = evklid(a, b);
    int d = a * p.first + b * p.second;
    return d;
}

