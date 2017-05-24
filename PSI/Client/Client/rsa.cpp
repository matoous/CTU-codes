/*******************************************************************
project:         PSI semestral project by Matous Dzivjak
(C) Copyright:   2017 - 2017 by Matous Dzivjak
e-mail:          dzivjmat@fel.cvut.cz
license:         MIT
Feel free to copy as long as you wont use it for the homework
*******************************************************************/

#include "rsa.h"
#include <iostream>
#include <stdlib.h>
#include <random>

using namespace std;

bignum_t modulo(bignum_t n, bignum_t p)
{
	bignum_t r = n%p;
	if (((p > 0) && (r < 0)) || ((p < 0) && (r > 0)))
		r += p;
	return r;
}

bignum_t modInverse(bignum_t n, bignum_t p) {
	n = modulo(n, p);
	for (bignum_t x = 1; x < p; x++) {
		if (modulo(n*x, p) == 1) return x;
	}
	return 0;
}

bignum_t gcd(bignum_t a, bignum_t b)
{
	for (;;)
	{
		if (a == 0) return b;
		b %= a;
		if (b == 0) return a;
		a %= b;
	}
}

bignum_t lcm(bignum_t a, bignum_t b)
{
	bignum_t temp = gcd(a, b);

	return temp ? (a / temp * b) : 0;
}

Rsa::Rsa(bignum_t a, bignum_t b) {
	p = a;
	q = b;
	n = p*q;
	totient = lcm((p - 1), (q - 1));
	set = false;
	e = rand() % (totient - 2 + 1) + 2;
	while (gcd(e, totient) != 1) {
		e++;
		if (e == totient)
			e = rand() % (totient - 2 + 1) + 2;
	}
	bignum_t k = 2;
	d = modInverse(e, totient);
}

void Rsa::set_public(bignum_t n, bignum_t e) {
	public_n = n;
	public_e = e;
	set = true;
}

bignum_t Rsa::decrypt(bignum_t msg) {
	if (!set) {
		cerr << "ERROR public key not set.\n" << endl;
		return 0;
	}
	return modpow(msg, public_e, public_n);
}

bignum_t Rsa::encrypt(bignum_t msg) {
	return modpow(msg, d, n);
}

bignum_t Rsa::getN() {
	return n;
}

bignum_t Rsa::getE() {
	return e;
}
