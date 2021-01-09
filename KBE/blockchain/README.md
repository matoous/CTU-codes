# Bitcoin task
The goal of this lab is to get familiar with blockchain (and bitcoin as the famous example of this technology) and brute force attacks.

There exists a fairly popular site https://www.bitaddress.org which allows you to generate new bitcoin addresses in your browser. For the purposes of this homework we have modified the site and introduced a vulnerabiliy and upload the vulnerable code here.

The task is to find a private key for existing bitcoin wallet with real money on it. Based on your personal preferences you can follow either of following stories.

## Story 1 - Good guy (at least at beggining?)
There are multiple incidents of bitcoin stealing - as this is not actually unpossible (today), guys just had to share their private keys, right? Well, most of them used unofficial clone of bitaddress.org.html website to generate their keys. As the algorithm is written in java-script and included into html website, you can run the code localy. Many of them did so and still - someone rob their money! Get in touch with the code and find how it is possible. For which address(es) with real money did you find a private key? Will you aware a community? (Will you grap the money?)

## Story 2 - Evil guy
There is popular key generator [NOTE: modified for purpose of this exercise]. It is used by many guys to (securely) get their own unique keys. Well, securely. But, can you look into the code and find vulnerability? Can you somehow brute force generated keys and find some with money? Let's steal some money!

# The task
You will complete the homework by sending the correct private key to me via email along with **text description and code** of your solution.
