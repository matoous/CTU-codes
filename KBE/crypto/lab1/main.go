package main

import (
	"bytes"
	"encoding/base64"
	"fmt"
	"io/ioutil"
	"math"
	"strings"
	"unicode"
)

var EnglishFrequencyMap = map[byte]float64{'a': 8.167, 'b': 1.492, 'c': 2.782, 'd': 4.253, 'e': 12.702, 'f': 2.228,
	'g': 2.015, 'h': 6.094, 'i': 6.966, 'j': 0.153, 'k': 0.772, 'l': 4.025,
	'm': 2.406, 'n': 6.749, 'o': 7.507, 'p': 1.929, 'q': 0.095, 'r': 5.987,
	's': 6.327, 't': 9.056, 'u': 2.758, 'v': 0.978, 'w': 2.360, 'x': 0.150,
	'y': 1.974, 'z': 0.074}

func main() {
	//mustReadHexFile := func(fileName string) string {
	//	f, err := ioutil.ReadFile(fileName)
	//	if err != nil {
	//		panic(err)
	//	}
	//	fs, err := hex.DecodeString(strings.ReplaceAll(string(f), "\n", ""))
	//	if err != nil {
	//		panic(err)
	//	}
	//	return string(fs)
	//}
	//
	//firstLine := func(text string) string {
	//	line := strings.Split(text, "\n")[0]
	//	if len(line) > 124 {
	//		line = line[0:124]
	//	}
	//	return line
	//}
	//
	//fmt.Printf("\n\nTask 1:\n============\n")
	//
	//fmt.Println(
	//	hex.EncodeToString([]byte(XORWithKey("everything remains raw", "word up"))),
	//)
	//
	//fmt.Println(
	//	hex.EncodeToString([]byte(XORWithKey("the world is yours", "illmatic"))),
	//)
	//
	//fmt.Printf("\n\nTask 2:\n============\n")
	//
	//h, _ := hex.DecodeString("404b48484504404b48484504464d4848045d4b")
	//fmt.Println(XORWithKey(string(h), "$"))
	//
	//// read file
	//fs := mustReadHexFile("text1.hex")
	//
	//fmt.Printf("\n\nTask 3:\n============\n")
	//
	//// manual
	//for i := byte(0); i <= 254; i++ {
	//	if unicode.IsPrint(rune(i)) {
	//		fmt.Printf("%q: %s\n", string(i), firstLine(XORWithKey(fs, string(i))))
	//	}
	//}
	//
	//fmt.Printf("\n\nTask 4:\n============\n")
	//
	//// auto
	//singleLetterKey := singleLetterDecrypt(fs)
	//fmt.Println(singleLetterKey)
	//fmt.Println(firstLine(XORWithKey(fs, string(singleLetterKey))))
	//
	//fmt.Printf("\n\nTask 5:\n============\n")
	//
	//// read file
	//fs2 := mustReadHexFile("text2.hex")
	//key := XORDecrypt(fs2, 10)
	//fmt.Println(key)
	//fmt.Println(firstLine(XORWithKey(fs2, key)))
	//
	//fmt.Printf("\n\nTask 6:\n============\n")
	//
	//fs3 := mustReadHexFile("text3.hex")
	//key = XORDecrypt(fs3)
	//fmt.Println(key)
	//fmt.Println(firstLine(XORWithKey(fs3, key)))

	//fmt.Printf("%d %8b\n", '$', '$')
	//fmt.Printf("%d %8b\n", 'a', 'a')
	//fmt.Printf("%d %8b\n", 'a' ^ '$', 'a' ^ '$')
	//fmt.Printf("%c\n", 0x40 ^ '$')
	//fmt.Printf("%c\n", 0x50 ^ '$')

	fmt.Println("!")
	x := base64.NewDecoder(base64.StdEncoding, strings.NewReader("LwoePAQIHH9ZUAkCJQIUDzJTRA4fGlFefx8NECxFW0RGJgQXF39UWUBDDE8BOggXHCtFX1IVLAoFAHE="))
	data, err := ioutil.ReadAll(x)
	if err != nil {
		panic(err)
	}
	fmt.Printf("%s\n", data)
	fmt.Println(XORWithKey(string(data), "Welcome <b>dzivjmat</b>, this is your first secret message."))
}

func splice(s string, offset, step int) string {
	var res strings.Builder
	res.Grow(len(s)/step+1)
	for i := offset; i < len(s); i += step {
		res.WriteByte(s[i])
	}
	return res.String()
}

// XORDecrypt breaks xor encrypted text (or at least tries to 🤷).
// If keyLength is specified only keys of given length are considered, otherwise we make well educated guess at the key
// length.
func XORDecrypt(text string, keyLength ...int) string {
	var keyLen int
	if len(keyLength) == 0 {
		keyLen = FindKeyLength(text)
	} else {
		keyLen = keyLength[0]
	}
	var key strings.Builder
	key.Grow(keyLen)
	for i := 0; i < keyLen; i++ {
		tmp := splice(text, i, keyLen)
		charKey := singleLetterDecrypt(tmp)
		key.WriteByte(charKey)
	}
	return key.String()
}

// FindKeyLength determines the most probable key length used to encrypt the text using index of coincidence.
func FindKeyLength(text string) int {
	best, bestIOC := 0, math.MaxFloat64
	for i := 0; i < 24; i++ {
		deltaIOC := .0
		for j := 0; j < i; j++ {
			deltaIOC += IOC(splice(text, j, i))
		}
		tmpIOC := 1.73 - (deltaIOC / float64(i))
		if tmpIOC < bestIOC {
			best, bestIOC = i, tmpIOC
		}
	}
	return best
}

// IOC calculates index of coincidence for the given text.
func IOC(letters string) float64 {
	counts := make(map[byte]int, 128)
	for _, char := range letters {
		// this won't work for non-ascii chars as we truncate to byte
		counts[byte(char)] += 1
	}
	ioc, textLength := .0, float64(len(letters))
	for _, count := range counts {
		count := float64(count)
		ioc += (count * (count - 1)) / (textLength * (textLength - 1) / 26)
	}
	return ioc
}

// singleLetterDecrypt decrypts single letter xor encrypted text by finding the key that decodes the text to text
// that resembles the english language the most.
func singleLetterDecrypt(s string) byte {
	scores := make(map[byte]float64, 255)
	for char := byte(0); char < 255; char++ {
		if unicode.IsPrint(rune(char)) { // consider only printable characters
			res := XORWithKey(s, string(char))
			scores[char] = FrequencyAnalysis(res)
		}
	}

	best, bestScore := byte(' '), .0
	for char, score := range scores {
		if score > bestScore {
			best, bestScore = char, score
		}
	}
	return best
}

// FrequencyAnalysis does frequency analysis of letters in the text and returns the score of similarity to english language.
func FrequencyAnalysis(s string) float64 {
	counts := make(map[byte]int, 64)
	for _, x := range s {
		counts[byte(x)] += 1
	}

	relativeFrequencies := make(map[byte]float64, len(counts))
	for k, v := range counts {
		relativeFrequencies[k] = float64(v) / float64(len(s))
	}

	score := .0
	for letter, relativeFrequency := range relativeFrequencies {
		if englishFrequency, ok := EnglishFrequencyMap[letter]; ok {
			score += math.Abs(englishFrequency - relativeFrequency)
		}
	}
	return score
}

// XORWithKey xor encrypts/decrypts given text with key.
func XORWithKey(text, key string) string {
	var res bytes.Buffer
	res.Grow(len(text))
	for i := 0; i < len(text); i++ {
		fmt.Printf("%c %d\n", text[i] ^ key[i%len(key)], text[i] ^ key[i%len(key)])
		res.WriteByte(text[i] ^ key[i%len(key)])
	}
	return res.String()
}
