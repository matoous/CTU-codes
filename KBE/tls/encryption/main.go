// package encryption contains code from crypto labs 1 and 2 adjusted for this task
package encryption

import (
	"bytes"
	"crypto/aes"
)

// XORWithKey xor encrypts/decrypts given text with key.
func XORWithKey(text, key []byte) []byte {
	var res bytes.Buffer
	res.Grow(len(text))
	for i := 0; i < len(text); i++ {
		res.WriteByte(text[i] ^ key[i%len(key)])
	}
	return res.Bytes()
}

func EncryptAES(key, plaintext []byte) []byte {
	if len(key) != 16 || len(plaintext) != 16 {
		panic("need 16 bytes for key and text")
	}
	c, err := aes.NewCipher(key)
	if err != nil {
		panic(err)
	}
	res := make([]byte, 16)
	c.Encrypt(res, plaintext)
	return res
}

func DecryptAES(key, ciphertext []byte) []byte {
	if len(key) != 16 || len(ciphertext) != 16 {
		panic("need 16 bytes for key and text")
	}
	c, err := aes.NewCipher(key)
	if err != nil {
		panic(err)
	}
	res := make([]byte, 16)
	c.Decrypt(res, ciphertext)
	return res
}

func EncryptAESECB(key, iv, text []byte) []byte {
	var res bytes.Buffer
	padded := pad(text)
	for i := 0; i < len(padded); i += 16 {
		iv = EncryptAES(key, XORWithKey(padded[i:i+16], iv))
		res.Write(iv)
	}
	return res.Bytes()
}

func DecryptAESECB(key, iv, text []byte) []byte {
	var res bytes.Buffer
	for i := 0; i < len(text); i += 16 {
		block := text[i : i+16]
		res.Write(XORWithKey(DecryptAES(key, block), iv))
		iv = block
	}
	return unpad(res.Bytes())
}

func pad(text []byte) []byte {
	n := 16 - (len(text) % 16)
	res := make([]byte, len(text)+n)
	copy(res, text)
	copy(res[len(text):], bytes.Repeat([]byte{byte(n)}, n))
	return res
}

func unpad(text []byte) []byte {
	n := int(text[len(text)-1])
	if n > 16 {
		return text
	}
	for i := len(text) - n; i < len(text); i++ {
		if text[i] != byte(n) {
			return text
		}
	}
	return text[0 : len(text)-n]
}
