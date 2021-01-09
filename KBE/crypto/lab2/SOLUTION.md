# lab2 Solution

```text
I, Matous Dzivjak, understand that cryptography is easy to mess up, and
that I will not carelessly combine pieces of cryptographic ciphers to
encrypt my users' data. I will not write crypto code myself, but defer to
high-level libaries written by experts who took the right decisions for me,
like NaCL.
```

## Exercise 2: encrypt single-block AES

> Write a function `encrypt_aes_block(x, key)` that takes an input `x` with a size
  of _exactly_ 16 bytes, and a `key` also 16 bytes long, and returns the result of
  encrypting this block with AES.
  
> _It should encrypt only one block_; make it fail
  (exception, assert...) if the input is not 16 byte long.
  
> Do not write the AES algo yourself (it is somewhat complex and not in the scope
  of this lab). Just defer to a library implementation that will encrypt the block
  for you. Maybe your library will force you to choose a block mode, in this case
  choose ECB and make sure you get only one block of output.
  
> What is the ciphertext of encrypting the plaintext `90 miles an hour` with the
  key `CROSSTOWNTRAFFIC`? Answer in hex.

The answer is `092fb4b0aa77beddb5e55df37b73faaa`.

## Exercise 3: decrypt single-block AES

> Write a function `decrypt_aes_block(y, key)` that takes an encrypted
  block `y` (exactly 16 bytes) and decrypts it with a 16 byte `key`. As above, this should only
  operate on 16 byte blocks, and should defer to a library implementation.
  
> What is the decryption of the 16 byte block `fad2b9a02d4f9c850f3828751e8d1565`
  with the key `VALLEYSOFNEPTUNE`?

The decrypted text is `I feel the ocean`.

## Exercise 4: implement PKCS&#35;7 padding

> Write a function `pad(x)` that takes input data with arbitrary size and returns
  the same data (or a copy of it) with some bytes appended to it, in this manner:
  ut should have, therefore, a length always a multiple of 16 bytes.

> What is the output of `pad("hello")`?

The output is 
```text
hello












```

(i.e. hello and 11 times `0x0B` - vertical tab)

## Exercise 5: implement PKCS#7 unpadding

> Write a function `unpad(y)` that takes an input data `y` padded with
  the scheme above, and returns the same data without the padding.
  
> What is the output of `unpad("hello\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b")`?

The output obviously is

```text
hello
```

## Exercise 6: implement ECB encryption

> Implement ECB mode yourself by following the steps above; you are not allowed to
  defer this task to your AES library of choice (but the library can still encrypt
  individual blocks through `encrypt_aes_ecb()`, of course).
 
> What is the encryption of the following plaintext? Use the key `vdchldslghtrturn`. Answer in hex.
  
>     Well, I stand up next to a mountain and I chop it down with the edge of my hand
  
The encrypted hex looks like this:
```text
883319258b745592ef20db9dda39b076a84f4955a48ba9caecd1583641cf3acac86acd5e5795de7895fab54481e9d8c3afc179c39412282eb8445ea2450e763df7282998a74baf19887c843b658f8891
```

## Exercise 7: implement ECB decryption

> Write a function `decrypt_aes_ecb(y, key)` that decrypts ECB by reversing the
  steps of the previous exercise: slice in 16 byte blocks, decrypt blocks
  independently, concatenate, unpad. Again, do not defer to your AES library for
  these steps, except for individual block decryption through
  `decrypt_aes_block()`.
  
> What is the decryption of the following ECB encoded ciphertext? Use the key `If the mountains`.
  
  ```text
  792c2e2ec4e18e9d3a82f6724cf53848
  abb28d529a85790923c94b5c5abc34f5
  0929a03550e678949542035cd669d4c6
  6da25e59a5519689b3b4e11a870e7cea
  ```     

The decryption is

```text
If the mountains fell in the sea / Let it be, it ain't me
```


## Exercise 8: ECB ciphertext manipulation (cut and paste 1)

> The file `text1.hex` contains lyrics of a song, where each line is exactly 32
  bytes long (31 letters + a newline character). In other words, the first line is
  in blocks 0-1; the second line is in block 2-3, etc.
  
> However, there is a small mistake! The first line and the third line have been
  unfortunately swapped, so the song is not correct anymore.

> 1. Have a quick look at the hex file. Can you quickly spot some obvious
    patterns? What fact can you deduce about the song lyrics?

1 line repeats multiple times, it will definitely be a POP-song with that
one easy catch phrase that is easy to remember!

> 2. At the end of the file, you can see a 16 byte block "alone"; however, all
     lines of the song lyrics are really 32 bytes long. Can you explain the
     presence of this last 16 byte block? Can you guess the plaintext of this
     block?

The text was padded.

> 3. Restore the correct order in the ciphertext. In other words, swap the first
     line and the third line. You must do this operation by manipulating the
     ciphertext only, without decrypting. (The point is to show you that you can
     manipulate encrypted text without knowing the key!)

Done.

> 4. Then, decrypt the text with key `TLKNGBTMYGNRTION`. The first line should
     start with "People" -- what is the rest of this line?

```text
People try to put us d-down    
```


## Exercise 9: ECB message crafting (cut and paste 2)

> 1. Write a function `welcome(name)` that 
      - first, concatenates three strings:
          - `"Your name is "` (13 bytes)
          - `name` (the input)
          - `" and you are a user"` (19 bytes)
      - then, encrypts the resulting string with the key `RIDERSONTHESTORM`
      - finally, returns the ciphertext

> For questions 2-5, only operate through the function `welcome()`; do
  not use the key directly. (Pretend you're an outside attacker, and
  therefore, you don't know the key.)

> 2. What is the ciphertext of `welcome("Jim")`? Answer in hex.

```text
d4d7730a2d4255c88dead80a2ad924f2b114fddb898d7ef8abdfefef30d552863f62b0605102e0186402df7666edcec7
```

> 3. Obtain the ciphertext of a block whose plaintext is 16 times the byte 16:
     `1010...10`.

Done.

> 4. Obtain the ciphertext of a block whose plaintext is "you are an admin".

Done.

> 5. Use these blocks (and perhaps other calls to `welcome`) to craft an encrypted
     message whose plaintext starts by `"Your name is "` and finishes with `" and you
     are an admin"`. In your report, write down your crafted ciphertext.

```text
4223cacc370040e004861fc1cb562b87c12c72c813087d43292ed445966edbd37edb62ceff6a92e3a59029a06e5e622b4e9eb1df207c25bebdcfc57385251689
```

> For question 6, you may use the key directly:

> 6. Decrypt your encrypted message to make sure it is correct. In your report,
     write the decrypted message.

```text
Your name is Matous Dzivjak and you are an admin
```

> 7. Could you quickly describe a real-world scenario where this could be a
     security issue?

We could for example change values in list of privileges if they were
encrypted this way. 🤷

## Exercise 10: ECB decryption (cracking) byte-at-a-time

See included `main.go` file for implementation.

## Bonus: when you have finished Ex. 10

> The server at breakme.zapto.org:9000 encrypts your text followed by a secret,
  just like in Ex 10. Small difference: it will wait for input bytes until it
  receives a newline "\\n", at which point it replies with the hex encoded
  ciphertext.  (So, when you bruteforce the secret characters, skip ASCII 10.)
  
> According to an authority in the domain of secrets, how have been "things"?

Can't connect to the server 😢
