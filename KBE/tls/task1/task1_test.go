package task1

import (
	"testing"

	"github.com/stretchr/testify/assert"

	"kbetls"
)

func TestTask1(t *testing.T) {
	bob := kbetls.NewAgent()
	alice := kbetls.NewAgent()
	bob.ReceivePublicKey(alice.SendPublicKey())
	alice.ReceivePublicKey(bob.SendPublicKey())
	assert.Equal(t, bob.CommonSecret, alice.CommonSecret, "bob and alice should have same secret key")
}
