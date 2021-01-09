package task8

import (
	"bytes"
	"math/big"
	"testing"

	"github.com/stretchr/testify/assert"

	"kbetls"
)

func TestTask8(t *testing.T) {
	// first override p and q with desired values for this task
	kbetls.P, _ = new(big.Int).SetString("19480788016963928122154998009409704650199579180935803274714730386316184054417141690600073553930946636444075859515663914031205286780328040150640437671830139", 10)
	kbetls.Q, _ = new(big.Int).SetString("17796969605776551869310475203125552045634696428993510870214166498382761292983903655073238902946874986503030958347986885039275191424502139015148025375449097", 10)

	message := "Trust no one"
	msgSHA1 := kbetls.GenerateMessageHash([]byte(message))
	publicKey, privateKey := kbetls.GenerateKeys(new(big.Int).SetInt64(3))
	signature := kbetls.GenerateSignature(privateKey, msgSHA1)
	assert.True(t, kbetls.VerifySignature(publicKey, signature, msgSHA1), "should be able to verify the valid signature")
	fs := FakeSignature(msgSHA1)
	assert.True(t, kbetls.VerifySignature(publicKey, fs, msgSHA1), "should be able to verify the faked signature")
}

func FakeSignature(sha1 []byte) []byte {
	diff := 128 - len(kbetls.ASN1) - len(sha1) - 4
	trash := bytes.Repeat([]byte("\xFF"), diff)

	var fakeUnsigned bytes.Buffer
	fakeUnsigned.WriteString("\x00\x01\xFF\x00")
	fakeUnsigned.Write(kbetls.ASN1)
	fakeUnsigned.Write(sha1)
	fakeUnsigned.Write(trash)
	fake := new(big.Int).SetBytes(fakeUnsigned.Bytes())

	cr, _ := kbetls.CubeRoot(fake)

	// there's a little issue with cube root, my implementation is off by 1 at the end compared to the python code
	// more about that here: https://stackoverflow.com/questions/30316639/arbitrary-precision-for-decimals-square-roots-in-golang
	// instead of rewriting the code in python I decided to hot fix it here because I am not sure I would
	// be able to come up with better solution
	cr = new(big.Int).Sub(cr, new(big.Int).SetInt64(1))

	return cr.Bytes()
}
