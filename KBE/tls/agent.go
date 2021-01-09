package kbetls

import (
	"crypto/rand"
	"crypto/sha1"
	"math/big"
)

// Agent is single agent capable of Diffie–Hellman communication.
type Agent struct {
	// publicData used for the message exchange
	publicData   *PublicData
	CommonSecret *big.Int // secret
	privateKey   *big.Int // private
	Message      string
}

// NewAgent creates new agent with default public data and randomly generated private key.
func NewAgent() *Agent {
	//Generate cryptographically strong pseudo-random between 0 - max
	pk, err := rand.Int(rand.Reader, defaultPublicData.P)
	if err != nil {
		panic(err)
	}
	return &Agent{
		publicData: defaultPublicData,
		privateKey: pk,
	}
}

// SetMessage sets the agents message. Used for testing.
func (a *Agent) SetMessage(msg string) {
	a.Message = msg
}

// PrivateKey returns agents private key.
func (a *Agent) PrivateKey() []byte {
	res := sha1.Sum(a.CommonSecret.Bytes())
	return res[:16]
}

// SendPublicData returns agents public data used for the cryptography.
func (a *Agent) SendPublicData() *PublicData {
	return a.publicData
}

// ReceivePublicData sets agents public data used for the cryptography.
func (a *Agent) ReceivePublicData(pd *PublicData) {
	a.publicData = pd
}

// SendPublicKey sends agent public key.
func (a *Agent) SendPublicKey() *big.Int {
	res := new(big.Int)
	res.Exp(a.publicData.G, a.privateKey, a.publicData.P)
	return res
}

// ReceivePublicKey receives public key of other exchange to establish the secure communication channel.
func (a *Agent) ReceivePublicKey(pk *big.Int) {
	a.CommonSecret = new(big.Int)
	a.CommonSecret.Exp(pk, a.privateKey, a.publicData.P)
}

// SendMessage sends the message encrypted using agents private key.
func (a *Agent) SendMessage() Message {
	iv := RandomIv()
	return Message{
		Data: Encrypt(a.PrivateKey(), iv, []byte(a.Message)),
		IV:   iv,
	}
}

// ReceiveMessage receives encrypted message, decrypts it using agents private key
// and saves it into the agents message variable.
func (a *Agent) ReceiveMessage(msg Message) {
	a.Message = string(Decrypt(a.PrivateKey(), msg.IV, msg.Data))
}
