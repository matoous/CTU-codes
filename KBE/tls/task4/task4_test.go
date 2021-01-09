package task4

import (
	"testing"

	"github.com/stretchr/testify/assert"

	"kbetls"
)

func TestTask4(t *testing.T) {
	alice := kbetls.NewAgent()
	bob := kbetls.NewAgent()

	alice.SetMessage("I'M 5UppER Kewl h4zKEr")
	assert.NotEmpty(t, alice.Message, "alice has da message")
	assert.Empty(t, bob.Message, "bob doesn't have da message")

	// Negotiate parameters publicly
	bob.ReceivePublicData(alice.SendPublicData())
	alice.ReceivePublicData(bob.SendPublicData())

	// Exchange keys publicly
	bob.ReceivePublicKey(alice.SendPublicKey())
	alice.ReceivePublicKey(bob.SendPublicKey())

	// Pass da message
	bob.ReceiveMessage(alice.SendMessage())
	// Bob has it now
	assert.Equal(t, alice.Message, bob.Message, "bob should have the message now (after the exchange)")
}
