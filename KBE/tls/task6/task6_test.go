package task6

import (
	"math/big"
	"testing"

	"github.com/stretchr/testify/assert"

	"kbetls"
)

func TestTask6(t *testing.T) {
	message := "I will not write crypto code myself, but defer to high-level libraries written by experts who took the right decisions for me"
	pub, priv := kbetls.GenerateKeys(new(big.Int).SetInt64(3))
	// test basic RSA encryption and decryption
	assert.Equal(t, []byte(message), kbetls.RSADecrypt(priv, kbetls.RSAEncrypt(pub, []byte(message))))
}
