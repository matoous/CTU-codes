package kbetls

import (
	"crypto/rand"
	"crypto/sha1"
	"math/big"
)

// MITM is man-in-the-middle agent that can intercept messages "securely" exchanged between two other agents.
type MITM struct {
	Agent
	PK1 []byte
	PK2 []byte
}

// NewMITM creates new man-in-the-middle agent.
func NewMITM() *MITM {
	//Generate cryptographically strong pseudo-random between 0 - max
	pk, err := rand.Int(rand.Reader, defaultPublicData.P)
	if err != nil {
		panic(err)
	}
	return &MITM{
		Agent: Agent{
			publicData: defaultPublicData,
			privateKey: pk,
		},
	}
}

// ReceivePublicKey receives public key of other agents and later uses them to intercept the messages.
func (m *MITM) ReceivePublicKey(pk *big.Int) {
	// receiving data from the first party
	if len(m.PK1) == 0 {
		s := sha1.Sum(new(big.Int).Exp(pk, m.privateKey, m.publicData.P).Bytes())
		m.PK1 = s[:16]
	} else {
		s := sha1.Sum(new(big.Int).Exp(pk, m.privateKey, m.publicData.P).Bytes())
		m.PK2 = s[:16]
	}
}

// InterceptMessage intercepts encrypted message exchanged between two external parties, decrypts it and saves it into
// the MITM agents Message variable.
func (m *MITM) InterceptMessage(msg Message) Message {
	m.Message = string(Decrypt(m.PK1, msg.IV, msg.Data))

	iv := RandomIv()
	return Message{
		IV: iv,
		Data: Encrypt(m.PK2, iv, []byte(m.Message)),
	}
}
