package kbetls

import (
	"crypto/rand"
	"math/big"

	"kbetls/encryption"
)

var defaultPublicData *PublicData

func init() {
	p, _ := new(big.Int).SetString("FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD129024E088A67CC74020BBEA63B139B22514A08798E3404DDEF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7EDEE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3DC2007CB8A163BF0598DA48361C55D39A69163FA8FD24CF5F83655D23DCA3AD961C62F356208552BB9ED529077096966D670C354E4ABC9804F1746C08CA18217C32905E462E36CE3BE39E772C180E86039B2783A2EC07A28FB5C55DF06F4C52C9DE2BCBF6955817183995497CEA956AE515D2261898FA051015728E5A8AACAA68FFFFFFFFFFFFFFFF", 16)
	g, _ := new(big.Int).SetString("2", 16)
	defaultPublicData = &PublicData{
		P: p,
		G: g,
	}
}

// PublicData are the public data exchanged between agents to establish secure communication.
type PublicData struct {
	P *big.Int // modulus
	G *big.Int // base
}

// Message is the message exchanged between agents. It contains the encrypted message and initialization vector.
type Message struct {
	Data []byte
	IV []byte
}

// Encrypt encrypts the message. The function accepts private key, initialization vector and the unencrypted message.
func Encrypt(key, iv, message []byte) []byte {
	return encryption.EncryptAESECB(key, iv, message)
}

// Decrypt decrypts the message. The function accepts private key, initialization vector and the unencrypted message.
func Decrypt(key, iv, message []byte) []byte {
	return encryption.DecryptAESECB(key, iv, message)
}

// RandomIv generates random initialization vector.
func RandomIv() []byte {
	iv := make([]byte, 16) // random initial vector
	_, err := rand.Read(iv)
	if err != nil {
		panic(err)
	}
	return iv
}
