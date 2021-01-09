package task2

import (
	"fmt"
	"testing"

	"github.com/stretchr/testify/require"

	"kbetls"
)

func TestTask2(t *testing.T) {
	bob := kbetls.NewAgent()
	alice := kbetls.NewAgent()
	bob.ReceivePublicKey(alice.SendPublicKey())
	alice.ReceivePublicKey(bob.SendPublicKey())
	privKey := bob.PrivateKey()
	require.Len(t, privKey, 16, "private key should be 16 bytes long")
	// we can't check the result as we use cryptographically strong randomness generator with nondeterministic result.
	fmt.Println(privKey)
}