# lab1 Solution

## Exercise 0: make utilities

Not needed, these things are easier in Golang 😉😁.

## Exercise 1: encrypt xor

> Write a function that encrypts a text with a xor key. The idea is
  simple: to obtain the first byte of the ciphertext, xor the first byte
  of the text and the first byte of the key. Continue in this fashion
  until the text is exhausted. If the key is shorter than the text, it
  should be recycled (start over from the beginning).
  
> For instance, xorring the text `everything remains raw` with the key
  `word up` should give you the following hexadecimal ciphertext:
  `121917165901181e01154452101d16061c1700071100`.
  
> What is the ciphertext of the hex-encoded text `the world is yours` against the key `illmatic`?


The function for encrypting text using key can look for example like this
in golang:

```go
// XORWithKey xor encrypts/decrypts given text with key.
func XORWithKey(text, key string) string {
	var res bytes.Buffer
	res.Grow(len(text))
	for i := 0; i < len(text); i++ {
		res.WriteByte(text[i] ^ key[i%len(key)])
	}
	return res.String()
}
```

We can verify that our function works on the example:

```go
fmt.Println(hex.EncodeToString([]byte(XORWithKey("everything remains raw", "word up"))))
```

Which indeed prints `121917165901181e01154452101d16061c1700071100`.
We can then encrypt `the world is yours` against the key `illmatic`.

```go
fmt.Println(hex.EncodeToString([]byte(XORWithKey("the world is yours", "illmatic"))))
```

Resulting into `1d04094d161b1b0f0d4c051e410d06161b1f`.

## Exercise 2: decrypt single-letter xor

> The following hex-encoded ciphertext was encoded against the
  single-letter key `$`, i.e. ASCII 36.
  
>    `404b48484504404b48484504464d4848045d4b`
  
> Before decrypting, shortly explain what pattern(s) are present in this
  ciphertext due to the weak mode of encryption.
  
> Then, decrypt the text. What is the plaintext?

What patterns are present? Well, we see a lot of the bytes from the original
text encoded as `4*` (i.e. starting with `4`). This is caused by following:
single byte key `$` is XORed with some unknown text. We can XOR `$` - `36 = (00100100)_b`
with `0x40` and `0x4f` to get the range of characters that will be encrypted
as bytes that are then hex encoded into `4*`. `'$' ^ (40)_16 = '$' ^ (01000000)_2 = 'd'` for
the lower bound (inclusive) and `'$' ^ (50)_16 = '$' ^ (01010000)_2 = 't'` for the upper bound (exclusive).
So the reason we see the `4*` pattern in the encrypted hex string is because the
text that was encrypted contains a lot of characters from range `d - t` and only single byte
was uesd for the encryptin key.

Decrypted text is `dolla dolla bill yo` which indeed contains a lot of characters from the
`d - t` range.

## Exercise 3: hand crack single-letter xor

> The file `text1.hex` contains a hex-encoded ciphertext that was
  xor encoded with a single letter.
  
> Decrypt it. What is the first line?

```text
Busta Rhymes up in the place, true indeed
```

## Exercise 4: automate cracking single-letter xor

> Solve the previous exercise, but instead of searching for the correct
  key/plaintext with your eyes, make the computer do it. In other words,
  you should have a function that, given a single-letter xor encoded
  ciphertext, will return you the single-byte key (and, if you want, the
  plaintext).
  
> You could devise a scoring function that checks, for a given
  decryption, if it seems like English. Then just iterate through all
  possible keys and return the key whose decryption gets the best
  score.

Did what I was told to (see included code) and got the correct result.

## Exercise 5: crack multiple-letter xor with given key length

> The file `text2.hex` contains a hex-encoded ciphertext that was xor
  encoded against a multiple-letter key -- just like Ex 1.

> Crack it. You are given the following indication: the key contain
  10 characters.

> Notice that by a simple manipulation of the ciphertext, the 10-letter
  encryption is nothing more than a collection of 10 single-letter
  encryptions -- which you can already crack thanks to Ex 4.

> What is the key, what is the first line?

The key is `SupremeNTM` and the first line is

```text
C'est le nouveau, phenomenal, freestyle du visage pale
```

## Exercise 6: crack multiple-letter xor with unknown key length

> Decrypt `text3.hex`, except this time you don't know the keylength.
  Even better if you can make your code find out the keylength before
  trying to decrypt.
  
> What is the key, what is the first line?

The key is `CL4SS!C_TIM3L35S` and the first line is

```text
And now for my next number I'd like to return to the...
```

## Bonus: when you have finished all exercises

> A careless user of cryptography has reused a classic timeless key to
  encrypt the file `secret.zip`, which reveals the way to an important
  philosophical work.
  
> According to this masterpiece, what comes brand new?

_Classic timeless key_, what could that be? Let's try `CL4SS!C_TIM3L35S`
from previous task. Yeah... we get file `philosophy.txt` with text

```text
PMbELEUfmIA
```
