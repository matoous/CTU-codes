# Lab 1

You can perform the following exercises in the language of your
choice, preferably a scripting language, but not necessarily. If you
are not sure, you should probably pick Python 3.

The exercises build on top of one another, so you are advised to solve
the exercises in the same file, in different functions.

## Exercise 0: make utilities

This exercise is not strictly mandatory, but it will be useful for the
rest of the lab.

Write 6 functions `bin2txt`, `bin2hex`, `txt2bin`, `hex2bin`, `hex2txt`, `txt2hex`, that convert between the following representations:
  - hex: `"426f6f6d"` (more precisely, a string to be interepreted as
      hexadecimal)
  - text: `"Boom"`
  - binary: `b"Boom"` in Python

Depending on the language, you may not have to distinguish between
binary and text, for instance in C it is the same thing, however in
Python one has type `str` whereas the other has type `bytes`.

You are not expected to write any complex algorithm here, just
delegate to the correct utility functions of your language if they are
provided. In other words don't rewrite yet another routine to
hand-parse hexadecimal.


## Exercise 1: encrypt xor

Write a function that encrypts a text with a xor key. The idea is
simple: to obtain the first byte of the ciphertext, xor the first byte
of the text and the first byte of the key. Continue in this fashion
until the text is exhausted. If the key is shorter than the text, it
should be recycled (start over from the beginning).

For instance, xorring the text `everything remains raw` with the key
`word up` should give you the following hexadecimal ciphertext:
`121917165901181e01154452101d16061c1700071100`.

What is the ciphertext of the hex-encoded text `the world is yours` against the key `illmatic`?


## Exercise 2: decrypt single-letter xor

The following hex-encoded ciphertext was encoded against the
single-letter key `$`, i.e. ASCII 36.

    404b48484504404b48484504464d4848045d4b

Before decrypting, shortly explain what pattern(s) are present in this
ciphertext due to the weak mode of encryption.

Then, decrypt the text. What is the plaintext?

## Exercise 3: hand crack single-letter xor

The file `text1.hex` contains a hex-encoded ciphertext that was
xor encoded with a single letter.

Decrypt it. What is the first line?

## Exercise 4: automate cracking single-letter xor

Solve the previous exercise, but instead of searching for the correct
key/plaintext with your eyes, make the computer do it. In other words,
you should have a function that, given a single-letter xor encoded
ciphertext, will return you the single-byte key (and, if you want, the
plaintext).

You could devise a scoring function that checks, for a given
decryption, if it seems like English. Then just iterate through all
possible keys and return the key whose decryption gets the best
score.

## Exercise 5: crack multiple-letter xor with given key length

The file `text2.hex` contains a hex-encoded ciphertext that was xor
encoded against a multiple-letter key -- just like Ex 1.

Crack it. You are given the following indication: the key contain
10 characters.

Notice that by a simple manipulation of the ciphertext, the 10-letter
encryption is nothing more than a collection of 10 single-letter
encryptions -- which you can already crack thanks to Ex 4.

What is the key, what is the first line?

## Exercise 6: crack multiple-letter xor with unknown key length

Decrypt `text3.hex`, except this time you don't know the keylength.
Even better if you can make your code find out the keylength before
trying to decrypt.

What is the key, what is the first line?

## Bonus: when you have finished all exercises

A careless user of cryptography has reused a classic timeless key to
encrypt the file `secret.zip`, which reveals the way to an important
philosophical work.

According to this masterpiece, what comes brand new?


