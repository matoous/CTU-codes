package task5

import (
	"testing"

	"github.com/stretchr/testify/assert"

	"kbetls"
)

func TestTask5(t *testing.T) {
	alice := kbetls.NewAgent()
	bob := kbetls.NewAgent()

	alice.SetMessage("I'M 5UppER Kewl h4zKEr")

	mallory := kbetls.NewMITM()

	// Alice has da message, Bob doesn't, Mallory neither
	assert.NotEmpty(t, alice.Message, "alice has da message")
	assert.Empty(t, bob.Message, "bob doesn't have da message")
	assert.Empty(t, mallory.Message, "mallory doesn't have da message")

	// Negotiate parameters publicly
	mallory.ReceivePublicData(alice.SendPublicData())
	bob.ReceivePublicData(mallory.SendPublicData())
	mallory.ReceivePublicData(bob.SendPublicData())
	alice.ReceivePublicData(mallory.SendPublicData())

	// Exchange keys publicly
	mallory.ReceivePublicKey(alice.SendPublicKey())
	bob.ReceivePublicKey(mallory.SendPublicKey())
	mallory.ReceivePublicKey(bob.SendPublicKey())
	alice.ReceivePublicKey(mallory.SendPublicKey())

	// Pass da message
	bob.ReceiveMessage(mallory.InterceptMessage(alice.SendMessage()))

	// Bob has it now
	assert.Equal(t, alice.Message, bob.Message, "bob should have the message now (after the exchange)")
	// Mallory too
	assert.Equal(t, alice.Message, mallory.Message, "mallory should have the message now (through interception)")
}
