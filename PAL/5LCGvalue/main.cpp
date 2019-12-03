#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <limits>
#include <algorithm>

using namespace std;

pair<int64_t, int64_t> gcdExtended(int64_t a, int64_t b) {
    if (a == 0) // default case
        return make_pair(0, 1);
    pair<int64_t, int64_t> gcd = gcdExtended(b % a, a); // swap and modulo
    return make_pair(gcd.second - (b / a) * gcd.first, gcd.first);
}

vector<int64_t> primeFactors(int64_t n) {
    vector<int64_t> factors;

    if (n % 2 == 0) {
        factors.push_back(2);
        while (n % 2 == 0) n = n / 2;
    }
    for (int64_t i = 3; i <= sqrt(n); i = i + 2) {
        if (n % i == 0) {
            factors.push_back(i);
            while (n % i == 0) n = n / i;
        }
    }

    if (n > 2) // we are left with some prime
        factors.push_back(n);

    return factors;
}

int main() {
    int64_t A, M, C, x0, x1, x2, inverse;
    cin >> M >> x1 >> x2;

    vector<int64_t> factors = primeFactors(M);
    reverse(factors.begin(), factors.end()); // big number will be more restrictive so start with them
    int64_t N = 0, X_max = numeric_limits<int64_t>::min(), X_min = numeric_limits<int64_t>::max();
    for (A = 2; A <= M; A++) {
        for (const int64_t &f : factors)// A is divisible by each prime factor of M
            if ((A - 1) % f != 0)
                goto next; // break wouldn't work here
        if (M % 4 == 0 && (A - 1) % 4 != 0) // if 4 divides M then also 4 divides A
            goto next;
        C = (x2 - x1 * A) % M;
        inverse = gcdExtended(A, M).first;
        x0 = (inverse * (x1 - C)) % M;
        if (x0 < 0) x0 += M;
        if (x0 > X_max) X_max = x0;
        if (x0 < X_min) X_min = x0;
        N++;
        next:;
    }
    cout << N << " " << X_min << " " << X_max << endl;
}
