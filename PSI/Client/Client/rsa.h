//============================================================================
// Name        : rsa.h
// Author      : Matous Dzivjak
// Version     : 0.1
// Copyright   : 2017, Matous Dzivjak
// Description : RSA utility for PSI semestral project
//============================================================================

#include <inttypes.h>

#define bignum_t uint64_t

class Rsa {
private:
	bignum_t p, q, n, cnt, totient, e, d, public_e, public_n;
	bool set;
	bignum_t gcd(bignum_t a, bignum_t h)
	{
		bignum_t temp;
		while (1) {
			temp = a%h;
			if (temp == 0)
				return h;
			a = h;
			h = temp;
		}
	}
	bignum_t modpow(bignum_t base, bignum_t exp, bignum_t modulus) {
		base %= modulus;
		bignum_t result = 1;
		while (exp > 0) {
			if (exp & 1) result = (result * base) % modulus;
			base = (base * base) % modulus;
			exp >>= 1;
		}
		return result;
	}
public:
	Rsa(bignum_t a, bignum_t b);
	void set_public(bignum_t n, bignum_t e);
	bignum_t encrypt(bignum_t msg);
	bignum_t decrypt(bignum_t msg);
	bignum_t getN();
	bignum_t getE();
};