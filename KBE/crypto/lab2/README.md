# Lab 2

Today we are going to explore practical attacks on block crypto.

The AES standard is a very robust way of encrypting data in "blocks", that is:
what you feed to the cipher must have a specific input size (always 16 bytes).
As of today, AES itself has never been publicly broken, which makes it one of
the most secure ciphers on Earth.

However! Even if you use this super-secure piece of computer science and apply
it in a bad way, you can end up with a very poor crypto system, which can be
cracked quite easily.

With AES, the problem is how to encrypt a message whose length exceeds 16 bytes
(it's not very useful if you are restricted to 16 bytes, right?) Engineers have
found different ways of splitting the input data into 16-byte blocks and then
use multiple block encryption in a separate manner. This is called a [Block
cipher mode of
operation](https://en.m.wikipedia.org/wiki/Block_cipher_mode_of_operation).
Today we will focus on the simplest one: ECB (Electronic Codebook),
which seems secure on the surface, and then attack it in two different ways:

  - manipulate the ciphertext to write new encrypted messages (without knowing
      the key)
  - decrypt an encrypted message (without knowing the key)

After this lab you will have one new skill: break some modern cryptography!

## Exercise 1: why we do all of this

This is the easiest exercise, but also the most important one.

Write in your report the following sentence:

    I, <your name here>, understand that cryptography is easy to mess up, and
    that I will not carelessly combine pieces of cryptographic ciphers to
    encrypt my users' data. I will not write crypto code myself, but defer to
    high-level libaries written by experts who took the right decisions for me,
    like NaCL.

That's it. You will indeed get points for writing this sentence in your report.

Some similar resources if the idea is not clear enough:

  - [Foot-shooting prevention agreement](http://www.moserware.com/assets/stick-figure-guide-to-advanced/aes_act_3_scene_02_agreement_1100.png) from [A Stick Figure Guide to the AES](http://www.moserware.com/2009/09/stick-figure-guide-to-advanced.html)
  - [Why is writing your own encryption discouraged?](https://crypto.stackexchange.com/questions/43272/why-is-writing-your-own-encryption-discouraged)
  - [One does not simply write their own cipher](https://security.stackexchange.com/questions/25585/is-my-developers-home-brew-password-security-right-or-wrong-and-why/25637#25637)
  - [Don't roll your own crypto.](https://security.stackexchange.com/questions/2202/lessons-learned-and-misconceptions-regarding-encryption-and-cryptology/2210#2210)

Here is a list of recommendations that you can read later if you want.

  - [Cryptographic right answers, 2018](https://latacora.micro.blog/2018/04/03/cryptographic-right-answers.html)
    Note how it just more or less says "do not code crypto yourself".
  - [NaCL](https://en.wikipedia.org/wiki/NaCl_(software)) and
  [libsodium](https://libsodium.gitbook.io/doc) what you should use if you use
  crypto in production

Finally, remember to stay up to date, crypto that was good yesterday can be
broken today.

---
 
That's it for the "soft" stuff. For the rest of the lab we will code that does
_not_ follow these rules, precisely so you can break it and see what happens
when one makes careless crypto decisions.

## Exercise 2: encrypt single-block AES

Write a function `encrypt_aes_block(x, key)` that takes an input `x` with a size
of _exactly_ 16 bytes, and a `key` also 16 bytes long, and returns the result of
encrypting this block with AES.

_It should encrypt only one block_; make it fail
(exception, assert...) if the input is not 16 byte long.

Do not write the AES algo yourself (it is somewhat complex and not in the scope
of this lab). Just defer to a library implementation that will encrypt the block
for you. Maybe your library will force you to choose a block mode, in this case
choose ECB and make sure you get only one block of output.

What is the ciphertext of encrypting the plaintext `90 miles an hour` with the
key `CROSSTOWNTRAFFIC`? Answer in hex.

## Exercise 3: decrypt single-block AES

Write a function `decrypt_aes_block(y, key)` that takes an encrypted
block `y` (exactly 16 bytes) and decrypts it with a 16 byte `key`. As above, this should only
operate on 16 byte blocks, and should defer to a library implementation.

What is the decryption of the 16 byte block `fad2b9a02d4f9c850f3828751e8d1565`
with the key `VALLEYSOFNEPTUNE`?

## Exercise 4: implement PKCS&#35;7 padding

Write a function `pad(x)` that takes input data with arbitrary size and returns
the same data (or a copy of it) with some bytes appended to it, in this manner:

  - if the length (in bytes) is a multiple of 16 minus 1, append a single byte
    of value 1 (hex `01`).
  - if the length is a multiple of 16 minus 2, append two bytes of value 2
    (hex `0202`).
  - if the length is a multiple of 16 minus 3, append three bytes of value 3
    (hex `030303`).
  - ...
  - if the length is a multiple of 16 minus 15, append fifteen bytes of
    value 15 (hex `0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f`).
  - if the length is exactly a multiple of 16, append sixteen bytes of value
    16 (hex `10101010101010101010101010101010`).

Your output should have, therefore, a length always a multiple of 16 bytes.

What is the output of `pad("hello")`?

## Exercise 5: implement PKCS#7 unpadding

Write a function `unpad(y)` that takes an input data `y` padded with
the scheme above, and returns the same data without the padding.

What is the output of `unpad("hello\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b")`?

## Exercise 6: implement ECB encryption

Write a function `encrypt_aes_ecb(x, key)` that:

  - takes an arbitrary sized input `x`,
  - pads it so the length is a multiple of 16 byte,
  - cuts it to consecutive blocks of 16 bytes,
  - encrypts every block independently with `encrypt_aes_block()` from Ex. 2
  - concatenates the result

Implement ECB mode yourself by following the steps above; you are not allowed to
defer this task to your AES library of choice (but the library can still encrypt
individual blocks through `encrypt_aes_ecb()`, of course).

What is the encryption of the following plaintext? Use the key `vdchldslghtrturn`. Answer in hex.

    Well, I stand up next to a mountain and I chop it down with the edge of my hand


## Exercise 7: implement ECB decryption

Write a function `decrypt_aes_ecb(y, key)` that decrypts ECB by reversing the
steps of the previous exercise: slice in 16 byte blocks, decrypt blocks
independently, concatenate, unpad. Again, do not defer to your AES library for
these steps, except for individual block decryption through
`decrypt_aes_block()`.

What is the decryption of the following ECB encoded ciphertext? Use the key `If the mountains`.

    792c2e2ec4e18e9d3a82f6724cf53848
    abb28d529a85790923c94b5c5abc34f5
    0929a03550e678949542035cd669d4c6
    6da25e59a5519689b3b4e11a870e7cea


## Exercise 8: ECB ciphertext manipulation (cut and paste 1)

The file `text1.hex` contains lyrics of a song, where each line is exactly 32
bytes long (31 letters + a newline character). In other words, the first line is
in blocks 0-1; the second line is in block 2-3, etc.

However, there is a small mistake! The first line and the third line have been
unfortunately swapped, so the song is not correct anymore.

1. Have a quick look at the hex file. Can you quickly spot some obvious
   patterns? What fact can you deduce about the song lyrics?

2. At the end of the file, you can see a 16 byte block "alone"; however, all
   lines of the song lyrics are really 32 bytes long. Can you explain the
   presence of this last 16 byte block? Can you guess the plaintext of this
   block?

3. Restore the correct order in the ciphertext. In other words, swap the first
   line and the third line. You must do this operation by manipulating the
   ciphertext only, without decrypting. (The point is to show you that you can
   manipulate encrypted text without knowing the key!)

4. Then, decrypt the text with key `TLKNGBTMYGNRTION`. The first line should
   start with "People" -- what is the rest of this line?

## Exercise 9: ECB message crafting (cut and paste 2)

1. Write a function `welcome(name)` that 
    - first, concatenates three strings:
        - `"Your name is "` (13 bytes)
        - `name` (the input)
        - `" and you are a user"` (19 bytes)
    - then, encrypts the resulting string with the key `RIDERSONTHESTORM`
    - finally, returns the ciphertext

For questions 2-5, only operate through the function `welcome()`; do
not use the key directly. (Pretend you're an outside attacker, and
therefore, you don't know the key.)

2. What is the ciphertext of `welcome("Jim")`? Answer in hex.

3. Obtain the ciphertext of a block whose plaintext is 16 times the byte 16:
   `1010...10`.

4. Obtain the ciphertext of a block whose plaintext is "you are an admin".

5. Use these blocks (and perhaps other calls to `welcome`) to craft an encrypted
   message whose plaintext starts by `"Your name is "` and finishes with `" and you
   are an admin"`. In your report, write down your crafted ciphertext.

For question 6, you may use the key directly:

6. Decrypt your encrypted message to make sure it is correct. In your report,
   write the decrypted message.

7. Could you quickly describe a real-world scenario where this could be a
   security issue?


## Exercise 10: ECB decryption (cracking) byte-at-a-time

In this exercise, you will decrypt the content of a ciphertext without knowing
the key!

Let `SECRET` be the following constant: "this should stay secret".

Write a function `hide_secret(x)` that 

  - concatenates two pieces of data:
    - `x` (the input, which can contain non-ASCII characters)
    - `SECRET`
  - encrypts the result with the key `COOL T MAGIC KEY`
  - returns the ciphertext

To test your function, make sure `hide_secret("just listen find the magic key")`
returns the following ciphertext:

    45a306391112e09639cc44fa4d53c79e
    c90162749b6055bbc3d0811c0da6bd9b
    df3dccce5ff98e742ffdc33a1c8e84b9
    d47e0182d8fa07c9291b25d8dab01199

Now, perform the next questions by pretending you don't know the key: only
operate through `hide_secret()`.

Write a function that discovers the first character of the secret. Here is how
you can proceed:

1. Call `hide_secret()` with an input containing fifteen times (16-1) the
   character `A`: "AAAAAAAAAAAAAAA". In the first block to be encrypted, what
   will be the last plaintext byte?

2. Call `hide_secret()` with the following 16-byte inputs: "AAA...AA\\x00",
   "AAA...AA\\x01"...  In other words, iterate through all possibilities for the
   last byte of the plaintext, and check the resulting ciphertext against the
   first ciphertext block of question 1 above. If it matches, it means you have
   discovered the first character!

Now generalize this technique to obtain all subsequent characters:

3. Say you know some bytes at the beginning of the secret. To obtain the next
   byte, decrement the number of A's. When calling `hide_secret("AAA..AA")` the
   plaintext of the first block will look like:

<!-- -->

    AAA..AAxxxxy
           ^^^^----- you know this already
               ^---- crack this byte, you know how to do it!

Automate this in a loop. When you run out of A's, realize you can start back at
15 and attack the second block (and later, the third, and so on)

How to find out when you have the complete secret (i.e. the termination
condition for your loop):

  - You could continue cracking new bytes, but they will always have the value 1. Why?
  - The length of the response to your initial call `hide_secret("AAA..AA")`
    will be exactly `a + s + 1` where `a` is the number of A's (between 0 and 15) and `s` is how many secret bytes you know so far. Why is this true only
    when the secret is complete?


## Bonus: when you have finished Ex. 10

The server at breakme.zapto.org:9000 encrypts your text followed by a secret,
just like in Ex 10. Small difference: it will wait for input bytes until it
receives a newline "\\n", at which point it replies with the hex encoded
ciphertext.  (So, when you bruteforce the secret characters, skip ASCII 10.)

According to an authority in the domain of secrets, how have been "things"?























