package task9

import (
	"crypto/sha1"
	"encoding/hex"
	"fmt"
	"testing"

	"github.com/stretchr/testify/assert"

	"kbetls"
)

func TestTask9(t *testing.T) {
	key := kbetls.RecoverPrivateKey()
	sha := sha1.Sum([]byte(hex.EncodeToString(key.Bytes())))
	fmt.Println(sha)
	shaHex := fmt.Sprintf("0x%x", sha)
	fmt.Println(shaHex)
	assert.Equal(t, "0x8f96763dea794b79094eef4717ceb5f10631d634", shaHex, "should be able to recover the private key")
}
