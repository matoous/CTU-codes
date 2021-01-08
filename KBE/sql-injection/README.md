# SQL Injection

The goal of this exercise is to hack web application on <https://kbe.felk.cvut.cz> that is vulnerable to [SQL injection](https://en.wikipedia.org/wiki/SQL_injection). The whole attack is divided into a series of tasks that build on each other. Therefore, although not mandatory, it is recommended to solve the tasks one by one in the given order.

## Task 1: Login without password
On the main page, there is a login form that you need to pass through, without knowledge of password. As a username use your FEL login name. 

Hints:
- In the backend code, there is an SQL query verifying credentials: `SELECT username FROM users WHERE username = '$_POST[username]' AND password = SHA1('$_POST[password]' . '$salt')`. If this query returns an existing username, you are in. Otherwise, a wrong credentials warning is shown.
- Fill the username input (i.e. variable `$_POST[username]`) with a value such that the `WHERE` clause of the query is true for your username. Pay attention to apostrophe `'` characters.
- Use hash symbol `#` to comment out an unwanted rest of the query.

## Task 2: Find out your PIN
As you can see, your account is not only password-protected, but also PIN-protected. Try to find out your PIN using the vulnerability from the previous task.

Hints:
- Table `users` contains column `pin`.
- The `WHERE` clause can provide you a binary signal (successful login / wrong credentials).
- Check presence and position of individual digits [0-9] in the PIN string that is associated with your username. Use [SQL AND operator](https://www.w3schools.com/sql/sql_and_or.asp) in combination with [SQL LIKE operator](https://www.w3schools.com/sql/sql_like.asp) to do that.

## Task 3: Overcome One-Time-Password
PIN-protection didn't stop you? Easy-peasy? Well, try to defeat the next layer of protection --- [Time-based One-Time Password](https://en.wikipedia.org/wiki/Time-based_One-time_Password_algorithm) --- widely used industry standard for 2-factor authentication.

Hints:
- Attack vector: steal the secret key from the database (column `secret`), insert it into e.g. [Google Authenticator](https://en.wikipedia.org/wiki/Google_Authenticator) and generate valid OTP values.
- The PIN page welcomes you with your username. Could you force it to welcome you with your secret key?
- Results of two SQL queries can be merged using [UNION operator](https://www.w3schools.com/sql/sql_union.asp). Ensure that the original query result is empty and add a new query returning your secret key.

## Task 4: Exfiltrate a list of all usernames, passwords, salts, secrets and pins
Bored of reading secret messages? Let's do some harm. What about exfiltrating all data stored in the database?

Hints:
- Once you get in, there is another SQL injection vulnerability, which allows you to retrieve all the data easily.
- The pagination mechanism is based on the following SQL query: `SELECT date_time, base64_message_xor_key AS message FROM messages WHERE username = '$_SESSION[username]' LIMIT 1 OFFSET $_GET[offset]`.
- Modify the query via URL get parameter `offset`. Use [UNION operator](https://www.w3schools.com/sql/sql_union.asp) to add more rows to the above `SELECT`. Note that the number of columns for both SELECTs must be the same. A dummy column can be added this way: `SELECT password, 1 FROM ...`. Also, try to avoid exfiltration via the second column (i.e. `base64_message_xor_key`) as the data are processed by a decryption algorithm before printing. Putting non-encrypted data there would result in a mess. [SQL CONCAT function](https://www.w3schools.com/sql/func_sqlserver_concat.asp) can be handy in situations when we need to join multiple columns into one.

## Task 5: Crack your password hash
Do you want to be able to login as an ordinary user? Well, then you need to know your password in addition to your PIN and SECRET key. 

Hints: 
- Passwords of student accounts are [hashed](https://en.wikipedia.org/wiki/Cryptographic_hash_function) and [salted](https://en.wikipedia.org/wiki/Salt_(cryptography)) in the following way: `sha1($password . $salt)`, where $password is five characters long string consisting of lowercase letters and numbers. Write a script trying all possible combinations.

## Task 6: Crack teacher's password hash
No comment.

:warning: Warning :warning:: Do not use brute-force as the password is quite long. Online tools should be sufficient.

## Task 7: Explain why teacher's password is insecure despite it's length

## Task 8: Print a list of all table names and their columns
Hint: [INFORMATION_SCHEMA](https://dev.mysql.com/doc/refman/5.5/en/tables-table.html)

## Task 9: Derive xor key used for encoding your messages
As you might have noticed, the secret messages are stored in an encrypted form in the database, but before printing they are decrypted on the backend. Since you have access to both forms of messages, try to derive the [xor key](https://en.wikipedia.org/wiki/XOR_cipher) used for encoding/decoding your messages.

## [BONUS :hurtrealbad:] Task 10: Find out key used for encoding secure codes

## [BONUS :feelsgood:] Task 11: Steal Martin Rehak's secure code 
