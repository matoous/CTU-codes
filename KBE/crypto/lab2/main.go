package main

import (
    "bytes"
    "crypto/aes"
    "encoding/hex"
    "fmt"
    "io/ioutil"
    "os"
    "strings"
    "unicode"
)

func EncryptAES(key, plaintext []byte) []byte {
    if len(key) != 16 || len(plaintext) != 16 {
        panic("need 16 bytes for key and text")
    }
    c, err := aes.NewCipher(key)
    if err != nil {
        panic(err)
    }
    res := make([]byte, 16)
    c.Encrypt(res, plaintext)
    return res
}

func DecryptAES(key, ciphertext []byte) []byte {
    if len(key) != 16 || len(ciphertext) != 16 {
        panic("need 16 bytes for key and text")
    }
    c, err := aes.NewCipher(key)
    if err != nil {
        panic(err)
    }
    res := make([]byte, 16)
    c.Decrypt(res, ciphertext)
    return res
}

func mustHexDecode(s string) []byte {
    res, err := hex.DecodeString(s)
    if err != nil {
        panic(err)
    }
    return res
}

func pad(text []byte) []byte {
    n := 16 - (len(text) % 16)
    res := make([]byte, len(text)+n)
    copy(res, text)
    copy(res[len(text):], bytes.Repeat([]byte{byte(n)}, n))
    return res
}

func unpad(text []byte) []byte {
    n := int(text[len(text)-1])
    if n > 16 {
        return text
    }
    for i := len(text) - n; i < len(text); i++ {
        if text[i] != byte(n) {
            return text
        }
    }
    return text[0 : len(text)-n]
}

func EncryptAESECB(key, text []byte) []byte {
    padded := pad(text)
    for i := 0; i < len(padded); i += 16 {
        copy(padded[i:i+16], EncryptAES(key, padded[i:i+16]))
    }
    return padded
}

func DecryptAESECB(key, text []byte) []byte {
    for i := 0; i < len(text); i += 16 {
        copy(text[i:i+16], DecryptAES(key, text[i:i+16]))
    }
    return unpad(text)
}

func welcome(who string) []byte {
    text := fmt.Sprintf("Your name is %s and you are a user", who)
    return EncryptAESECB([]byte("RIDERSONTHESTORM"), []byte(text))
}

func main() {
    d, _ := hex.DecodeString("7280ED648FEDDB0E8BC8B12DA93723BF824AE15DC1DB8204238F3583FE5DB0CF")
    fmt.Println(len(d))
    fmt.Println(string(DecryptAESECB([]byte("UScnF4L5AX5kmpib"), d)))
    os.Exit(1)
    mustReadHexFile := func(fileName string) string {
        f, err := ioutil.ReadFile(fileName)
        if err != nil {
            panic(err)
        }
        fs, err := hex.DecodeString(strings.ReplaceAll(string(f), "\n", ""))
        if err != nil {
            panic(err)
        }
        return string(fs)
    }

    fmt.Println(hex.EncodeToString(EncryptAES([]byte("CROSSTOWNTRAFFIC"), []byte("90 miles an hour"))))
    fmt.Println(string(DecryptAES([]byte("VALLEYSOFNEPTUNE"), mustHexDecode("fad2b9a02d4f9c850f3828751e8d1565"))))
    fmt.Println(string(pad([]byte("hello"))))
    fmt.Println(unpad([]byte("hello\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b")))
    fmt.Println(hex.EncodeToString(EncryptAESECB([]byte("vdchldslghtrturn"), []byte("Well, I stand up next to a mountain and I chop it down with the edge of my hand"))))
    fmt.Println(string(DecryptAESECB([]byte("If the mountains"), mustHexDecode("792c2e2ec4e18e9d3a82f6724cf53848abb28d529a85790923c94b5c5abc34f50929a03550e678949542035cd669d4c66da25e59a5519689b3b4e11a870e7cea"))))
    fmt.Println(string(DecryptAESECB([]byte("TLKNGBTMYGNRTION"), []byte(mustReadHexFile("crypto/lab2/text1.hex")))))

    // 13 chars before name
    // 19 chars after name
    fmt.Println(hex.EncodeToString(welcome("Jim")))
    paddingCipher := welcome("")
    paddingCipher = paddingCipher[len(paddingCipher)-16:]
    fmt.Println(hex.EncodeToString(paddingCipher))
    adminCipher := welcome(fmt.Sprintf("   you are an admin"))
    adminCipher = adminCipher[16:32]
    fmt.Println(hex.EncodeToString(adminCipher))
    nameCipher := welcome("Matous Dzivjak and")
    fakeCipher := make([]byte, 64)
    copy(fakeCipher[0:32], nameCipher[0:32])
    copy(fakeCipher[32:48], adminCipher)
    copy(fakeCipher[48:64], paddingCipher)
    fmt.Println(hex.EncodeToString(fakeCipher))
    fmt.Println(string(DecryptAESECB([]byte("RIDERSONTHESTORM"), fakeCipher)))

    secret := "this should stay secret"
    hideSecret := func(in []byte) []byte {
        n := len(in) + len(secret)
        res := make([]byte, n)
        copy(res, in)
        copy(res[len(in):], secret)
        return EncryptAESECB([]byte("COOL T MAGIC KEY"), res)
    }
    fmt.Println(hex.EncodeToString(hideSecret([]byte("just listen find the magic key"))))

    findSecret := func() []byte {
        secret := make([]byte, 0)
        filling := bytes.Repeat([]byte{'A'}, 15)
        block := 1
        for {
            fillingCipher := hideSecret(filling)[:block*16]
            for i := byte(0); i < 255; i++ {
                // consider only printable characters, 0x01 is for terminal condition
                if unicode.IsPrint(rune(i)) || i == 0x01 {
                    guess := make([]byte, len(filling)+len(secret)+1)
                    copy(guess, filling)
                    copy(guess[len(filling):], secret)
                    copy(guess[len(filling)+len(secret):], []byte{i})
                    guessCipher := hideSecret(guess)[:block*16]
                    if bytes.Compare(fillingCipher, guessCipher) == 0 {
                        // terminal condition met
                        if i == 0x01 {
                            return secret
                        }
                        // we found next character of the secret
                        secret = append(secret, i)
                        if len(secret)%16 == 0 && len(secret) != 0 { // block filled
                            block += 1
                            filling = bytes.Repeat([]byte{'A'}, 15) // reset filling
                        } else {
                            filling = filling[:len(filling)-1]
                        }
                        break
                    }
                }
            }
        }
    }
    fmt.Println(string(findSecret()))
}
