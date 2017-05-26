/*******************************************************************
project:         PSI semestral project by Matous Dzivjak
(C) Copyright:   2017 - 2017 by Matous Dzivjak
e-mail:          dzivjmat@fel.cvut.cz
license:         MIT
Feel free to copy as long as you wont use it for the homework
*******************************************************************/

#include <inttypes.h>

#define bignum_t uint64_t

class Rsa {
private:
	bignum_t p, q, n, cnt, totient, e, d, public_e, public_n;
	bool set;
public:
	Rsa(bignum_t a, bignum_t b);
	void set_public(bignum_t n, bignum_t e);
	bignum_t encrypt(bignum_t msg);
	bignum_t decrypt(bignum_t msg);
	bignum_t getN();
	bignum_t getE();
};
