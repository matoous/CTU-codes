package task3

import (
	"crypto/rand"
	"encoding/hex"
	"testing"

	"github.com/stretchr/testify/require"

	"kbetls"
)

func TestTask3(t *testing.T) {
	// generate random initialization vector
	iv := kbetls.RandomIv()

	// generate random encryption key
	key := make([]byte, 16)
	_, err := rand.Read(key)
	require.NoError(t, err, "must be able to generate encryption key")

	b := make([]byte, 124) // random long text
	_, err = rand.Read(b)
	require.NoError(t, err, "must be able to generate message for testing")
	text := hex.EncodeToString(b)

	encrypted := kbetls.Encrypt(key, iv, []byte(text))
	decrypted := string(kbetls.Decrypt(key, iv, encrypted))

	require.Equal(t, text, decrypted, "decrypted text should be equal to original one")
}