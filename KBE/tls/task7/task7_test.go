package task7

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"math/big"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"

	"kbetls"
)

func TestTask7(t *testing.T) {
	// read the captured messages files
	data, err := ioutil.ReadFile("message_captured")
	require.NoError(t, err, "must be able to read message_captured")

	// adjust it so we can parse it into json
	data = []byte(fmt.Sprintf("[%s]", strings.Join(strings.Split(strings.TrimSpace(string(data)), "\n"), ",")))

	// read the file
	var messages []struct {
		Moduli *big.Int `json:"n"`
		E      *big.Int `json:"e"`
		Data   *big.Int `json:"data"`
	}
	err = json.Unmarshal(data, &messages)
	require.NoError(t, err, "must be able to decode the messages from message_captured file")

	// following lines implement the RSA broadcast attack
	datas := make([]*big.Int, 0, 3)
	modulis := make([]*big.Int, 0, 3)
	for i := 0; i < 3; i++ {
		datas = append(datas, messages[i].Data)
		modulis = append(modulis, messages[i].Moduli)
	}
	res := kbetls.ChineseRemainder(modulis, datas)
	cubeRoot, _ := kbetls.CubeRoot(res)

	// verify the result
	want := "Even experts make mistakes. Crypto is extremely difficult to get right"
	assert.Equal(t, want, string(cubeRoot.Bytes()), "should successfully perform the RSA broadcast attack")
}
