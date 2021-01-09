# Lab 4

Today we will attack a very popular PRNG: the Mersenne Twister. It is a very good PRNG for
applications such as physics simulations, statistics, etc. But in its original version, it is not
designed for cryptographic applications, and we are going to see why.

All integers below (except indices) are unsigned 32-bit integers. We use them in big-endian format:
for instance, decimal 1234567890 is the same as 0x499602d2.


## Exercise 1: Write the Mersenne Twister

a. Prepare a class MT (name it as you want) holding:

  - an array of 624 integers (32 bit, unsigned): `v[]`
  - an `index` on the array.

  Also store the following constants:

      w,n,m,r = 32,624,397,31
      a = 0x9908B0DF
      u,d = 11, 0xFFFFFFFF
      s,b = 7,  0x9D2C5680
      t,c = 15, 0xEFC60000
      l = 18
      f = 1812433253

b. Implement the constructor `MT(seed)` to call the method `MT.seed(seed)` that we will implement
just below.

c. Implement `MT.seed(seed)`:
    
        v[0] = seed
        for i = 1..623
            v[i] = (f * (v[i-1] ^ (v[i-1] >> 30)) + i) & 0xffffffff
        index = n

  Check that your code is correct: `MT(seed=123456789)` has the following vector state after seeding:
  `[123456789, 2139825738, 2037464729, 1515522555, ..., 3075821708]`

d. Implement a stateless function `twist(v_prev) -> v`:

        v = clone v_prev
        for i = 0..623
            x = (v[i] & 0x80000000) ^ (v[(i+1)%n] & 0x7fffffff)
            xA = x >> 1
            if (x & 0x00000001):
                xA = xA ^ a
            v[i] = v[(i+m) % n] ^ xA
        return v

  Explain what the operations with masks 0x80000000 and 0x7fffffff mean.
  
  Explain why xA in the pseudo-code above contains the result of the multiplication:
  
  <img src="eqns/e7d6f71edc30f0e1e8730d5f0c956198.png" />
  
  where `x_i`, `a_i`  are bits (i.e. the multiplication takes place in F2, the field with two
  elements).
  
  Give a necessary and sufficient condition on the constant `a`, for A to be invertible. Is it
  satisfied by our constant a = 0x9908B0DF?


e. Write a stateless function `temper(x) -> z`:

        y1 = x  ^ ((x  >> u) & d)
        y2 = y1 ^ ((y1 << s) & b)
        y3 = y2 ^ ((y2 << t) & c)
        z  = y3 ^ (y3 >> l)
        return z

  (Just to be clear, here, `y1`, `y2`, `y3` are integers, not bits.)

f. Write a method `MT.next()`, used to output a new random number:

        if index == n:
            v = twist(v)
            index = 0
        x = v[index]
        index++
        return temper(x)

g. Test that with a seed of 123456789, you get the following sequence of random numbers: 2288500408,
4254805660, 2294099250, 56498137...



## Exercise 2. Reverse the Mersenne Twister

For this part you are not given any pseudo-code. You will have to think to figure out the answers!

a. Write `untemper(z) -> x`. You can go step by step:

  - `z -> y3` is easy
  - `y3 -> y2` is easy
  - `y2 -> y1` is more difficult. Take a pen and paper, draw bit diagrams. (Hint: start from the right
      part of y2...)
  - `y1 -> y0` is difficult too, but is solved with the same technique as above.

  To test it, make sure that `untemper(temper(x)) == x` for some x's of your choice.

b. Show that:

  <img src="eqns/4a53b347fbb57a600604fb5138693b65.png" />

c. Write `untwist(v) -> v_prev`, which takes an array of 624 integers and computes an array `v_prev`
such that `twist(v_prev) == v`.



## Exercise 3. Exploit the Mersenne Twister

a. Write a class PwResetTokenGen, which contains a MT seeded with whatever seed you give in the
constructor.

b. Write a method `PwResetTokenGen.gen_token() -> 64 bytes` that generates a 64-byte long
byte-string. Since the MT generates 4-byte ints, just concatenate their big-endian representation.

  Example: if you get the numbers 2288500408 (0x8867beb8), 4254805660 (0xfd9b2e9c), 2294099250
  (0x88bd2d32), 56498137 (0x035e17d9), ... join them into a byte-string whose hex representation is
  "8867beb8fd9b2e9c88bd2d32035e17d9...". In fact, this example is the beginning of the first token you
  will obtain with a seed of 123456789.

c. Write the following scenario, which represents a possible attack against a real service:

  - initialize a token generator with seed 123456789
  - "burn-in" the generator by generating, say, 1000 tokens that you throw away
  - generate a token, this time, store it in a variable like `secret_token`. Don't look at it!
  - recover the secret token by only issuing _new_ calls to the token generator...
  - verify you were right, by comparing your "recovered" token with the "true" secret token.


## Bonus: after your exploit in Ex.3c works

Attack the webserver at http://breakme.zapto.org:9004/

According to a peaceful poem, what do "Leaders" do?
