# Lab 3

Today the lab will focus on a side-channel attack on AES.

In Lab 2 you have discovered that AES-ECB was not a suitable mode for encrypting
messages. Therefore, nobody really uses ECB, so you may think that the exercises
of Lab 2 were kind of a theoretical exercise, not a real-world skill.

The main problem with ECB was that it always encrypts a given plaintext into the
same ciphertext, regardless of the block's position in the message. How can we
fix it?  One possible answer is CBC (Ciphertext Block Chaining). In this mode,
before we give a block of plaintext to AES, we xor it with the ciphertext we
obtained for the previous block. For the first block, since there is no previous
ciphertext, we use instead a special block called IV (Initialization Vector),
which is simply a block containing random data.

You will need some functions from lab 2, make sure they are in your environment
before beginning:

    - encrypt_aes_block()
    - decrypt_aes_block()
    - pad()
    - unpad()


## Exercise 1: implement CBC encryption

Write a function `encrypt_aes_cbc(x, key, iv)` that implements the encryption
part of the CBC block mode of operation. Pad the plaintext with your function
`pad()`, call AES through your function `encrypt_aes_block()` and implement the
remaining parts (xor plaintext against previous ciphertext) yourself.

You will find formulas and a good diagram at [Wikipedia:
CBC](https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation#Cipher_Block_Chaining_%28CBC%29).

What is the encryption of the message "we are always running for the thrill of it" (42 bytes) with the
key "WALKINGONADREAM." and the IV hex "a1b27b4eeef364f9da74a8c06edbd771"?

## Exercise 2: implement CBC decryption

Write a function `decrypt_aes_cbc(y, key, iv)` that implements the _decryption_
part. Same as above, use your functions `decrypt_aes_block()` and `unpad()`.

Then, take the ciphertext you obtained in Ex 1; decrypt it back and make sure
you obtain the original plaintext.

## Exercise 3: bitflip a single character

Consider the following plaintext: "welcome to this car" (19 bytes). After
padding, this fits in two blocks.

a. What is the first byte of the second block (i.e. `plaintext_blocks[1][0]`)?
What its ASCII representation in decimal, and in binary? If you wanted to turn
this byte to the letter 'b', which bit should you change?

b. Encrypt the plaintext with key "nckdlgyzsklvheba" and IV hex
`fbd71a63197605dde3ac8bce86c1ead7`. What is the ciphertext?

c. Manipulate the _ciphertext_ obtained in question b above, so that after
decryption, the byte changes to the letter 'b' _in the plaintext_. What is your
modified ciphertext?  Decrypt your modified ciphertext: what is the modified
plaintext?

d. Do the same as question c above, but instead of changing the byte to letter 'b',
change it to letter 'w' (the difference is that you are not swapping a single
bit anymore).

## Exercise 4: implement an IV generator

Write a function `generate_iv()` that generates a random block of 16
bytes. Don't write any complicated stuff: call a CSPRNG. Your language is
probably [listed
here](https://paragonie.com/blog/2016/05/how-generate-secure-random-numbers-in-various-programming-languages).

What does CSPRNG stand for? Can you explain in a few words what differs from a
(non-CS) PRNG? Why do you _always_ have a to use a CSPRNG in cryptographic
situations?

## Exercise 5: Server encryption and decryption oracle

a. write a function `server_encrypt()` with no arguments, that generates a new
IV, pads + encrypts the following secret: "this data is top secret, can you
decrypt it?" (44 bytes) Use the key: "nzbighzuxgjsoajg", and return the IV and
the ciphertext.

> Note: You can return a tuple `(iv,ciphertext)`, or some kind of struct
> `{iv=..., ciphertext=...}`, but you can also decide to concatenate them in a
> single byte-string: `iv || ciphertext`, which is quite common for
> communications involving crypto (in crypto, the symbol || means
> "concatenation"). Choose what you find most practical.

b. write a function `server_decrypt(y, iv)` that uses the same key to decrypt
ciphertext `y`. Do not return the plaintext: instead, return true if the
decryption was successful, and return false if the padding was invalid. (For
instance, the block 'aaaaaaaaaaaaaaa\\x02' has invalid padding.)

## Exercise 6: attack the oracle

Call `server_encrypt()` and obtain a piece of ciphertext with its IV, store it
somewhere for the whole exercise. Without knowing the key, we will crack the
integrality of its contents!

The "steps" below are there to help you write the cracking code through increasing
levels of complexity. You can reshape your code from step to step and only
send your finished code without the intermediate steps.

### Step 1: decrypt the last character of the first block

For now, keep only the IV and the first block.

Decrypt the last byte of the block using only calls to `server_decrypt(block, iv)`. Here
is how to do it:

  - modify the last byte of the IV by xorring it with a byte b=0..255
  - most of the times, the server will reject your ciphertext (i.e your function
      will return false).
  - but if it accepts it, it means that the padding was
      valid in the plaintext. Probably* you changed the plaintext block to
      'xxxxxxxxxxxxxxx\\x01'.
  - you can now deduce the last plaintext byte!

\* Probably, because it could also be 'xxxxxxxxxxxxxx\\x02\\x02', but that's less
likely, or 'xxxxxxxxxxxxx\\x03\\x03\\x03', or...

### Step 2: decrypt the full block

Decrypt the full block using only calls to `server_decrypt(block, iv)`.

  - if you know $i$ bytes on the right, you can manipulate the IV so as to fix
      the $i$ known bytes to be equal to some padding value $i$.
  - then you can iterate through b=0..255 and find the next byte, as above.
  - do this until you get all 16 bytes.

If no byte in 0..255 produces a valid padding, it means that you made a mistake
in a previous byte. See "\* probably" above. Backtrack and change your assumption
to the next "most likely" value: 2, 3, ...

### Step 3: decrypt all blocks

Decrypt the full message using only calls to `server_decrypt`.

You only need automate step 2 above for all blocks.








