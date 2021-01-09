package kbetls

import (
	"bytes"
	"crypto/sha1"
	"math/big"
)

var P *big.Int
var Q *big.Int

func init() {
	P, _ = new(big.Int).SetString("13604067676942311473880378997445560402287533018336255431768131877166265134668090936142489291434933287603794968158158703560092550835351613469384724860663783", 10)
	Q, _ = new(big.Int).SetString("20711176938531842977036011179660439609300527493811127966259264079533873844612186164429520631818559067891139294434808806132282696875534951083307822997248459", 10)
}

// lcm is helper function for finding the least common multiple of two big integers.
func lcm(a, b *big.Int) *big.Int {
	gcd := new(big.Int).GCD(nil, nil, a, b)
	lcm := new(big.Int).Mul(a, b)
	lcm = lcm.Div(lcm, gcd)
	return lcm
}

// PubKey is public key in RSA cryptography.
type PubKey struct {
	N *big.Int // modulus
	E *big.Int // exponent
}

// PrivKey is the private key in RSA cryptography.
type PrivKey struct {
	N *big.Int // modulus
	D *big.Int // decryption exponent
}

// GenerateKeys generates public and private key to be used in RSA cryptography.
func GenerateKeys(e *big.Int) (publicKey PubKey, privateKey PrivKey) {
	one := new(big.Int).SetInt64(1)
	n := new(big.Int).Mul(P, Q) // modulus
	t := lcm(new(big.Int).Sub(P, one), new(big.Int).Sub(Q, one))
	d := new(big.Int).ModInverse(e, t) // decryption exponent
	return PubKey{n, e}, PrivKey{n, d}
}

// RSAEncrypt encrypts message using given RSA public key.
func RSAEncrypt(pk PubKey, message []byte) []byte {
	return new(big.Int).Exp(new(big.Int).SetBytes(message), pk.E, pk.N).Bytes()
}

// RSADecrypt decrypts message using given RSA private key.
func RSADecrypt(pk PrivKey, message []byte) []byte {
	return new(big.Int).Exp(new(big.Int).SetBytes(message), pk.D, pk.N).Bytes()
}

// ChineseRemainder find chinese reminder of given integers and coprimes.
func ChineseRemainder(n, a []*big.Int) *big.Int {
	if len(n) != len(a) {
		panic("mismatch in lengths")
	}
	prod := new(big.Int).SetInt64(1)
	sum := new(big.Int).SetInt64(0)
	for i := range n {
		prod = new(big.Int).Mul(prod, n[i])
	}
	for i := range n {
		p := new(big.Int).Div(prod, n[i])
		inv := new(big.Int).ModInverse(p, n[i])
		tmp := new(big.Int).Mul(a[i], inv)
		sum = sum.Add(sum, new(big.Int).Mul(tmp, p))
	}
	return new(big.Int).Mod(sum, prod)
}

func GenerateMessageHash(msg []byte) []byte {
	sha := sha1.Sum(msg)
	return sha[:]
}

var ASN1 = []byte("\x30\x21\x30\x09\x06\x05\x2b\x0e\x03\x02\x1a\x05\x00\x04\x14")

func GenerateSignature(key PrivKey, sha1 []byte) []byte {
	diff := (key.N.BitLen())/8 - len(ASN1) - len(sha1) - 3
	padding := bytes.Repeat([]byte("\xFF"), diff)
	var unsignedData bytes.Buffer
	unsignedData.WriteByte('\x00')
	unsignedData.WriteByte('\x01')
	unsignedData.Write(padding)
	unsignedData.WriteByte('\x00')
	unsignedData.Write(ASN1)
	unsignedData.Write(sha1)
	return RSADecrypt(key, unsignedData.Bytes())
}

func VerifySignature(key PubKey, signature []byte, sha1 []byte) bool {
	res := RSAEncrypt(key, signature)
	diff := (key.N.BitLen() / 8) - len(res)
	pad := bytes.Repeat([]byte("\x00"), diff)
	res = append(pad, res...)
	res = res[:(key.N.BitLen())/8]

	// must start with "\x00\x01"
	if bytes.Compare(res[0:2], []byte("\x00\x01")) != 0 {
		return false
	}

	// padding
	i := bytes.Index(res[2:], []byte("\x00"))
	// SHA1 hash
	hashStart := i + len(ASN1) + 3
	signatureHash := res[hashStart : hashStart+20]
	return bytes.Compare(signatureHash, sha1) == 0
}
