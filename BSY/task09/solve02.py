#!/usr/bin/python
# -*- coding: utf8 -*-
import argparse
import socket
from dnslib import *
from base64 import b64decode, b32decode
import sys

#======================================================================================================
#                                            HELPERS FUNCTIONS
#======================================================================================================

#------------------------------------------------------------------------
# Class providing RC4 encryption/decryption functions
#------------------------------------------------------------------------
class RC4:
    def __init__(self, key = None):
        self.state = range(256) # initialisation de la table de permutation
        self.x = self.y = 0 # les index x et y, au lieu de i et j

        if key is not None:
            self.key = key
            self.init(key)

    # Key schedule
    def init(self, key):
        for i in range(256):
            self.x = (ord(key[i % len(key)]) + self.state[i] + self.x) & 0xFF
            self.state[i], self.state[self.x] = self.state[self.x], self.state[i]
        self.x = 0

    # Decrypt binary input data
    def binaryDecrypt(self, data):
        output = [None]*len(data)
        for i in range(len(data)):
            self.x = (self.x + 1) & 0xFF
            self.y = (self.state[self.x] + self.y) & 0xFF
            self.state[self.x], self.state[self.y] = self.state[self.y], self.state[self.x]
            output[i] = (data[i] ^ self.state[(self.state[self.x] + self.state[self.y]) & 0xFF])
        return bytearray(output)
        
#------------------------------------------------------------------------
def progress(count, total, status=''):
    """
    Print a progress bar - https://gist.github.com/vladignatyev/06860ec2040cb497f0f3
    """
    bar_len = 60
    filled_len = int(round(bar_len * count / float(total)))

    percents = round(100.0 * count / float(total), 1)
    bar = '=' * filled_len + '-' * (bar_len - filled_len)
    sys.stdout.write('[%s] %s%s\t%s\t\r' % (bar, percents, '%', status))
    sys.stdout.flush()

#------------------------------------------------------------------------
def fromBase64URL(msg):
    msg = msg.replace('_','/').replace('-','+')
    if len(msg)%4 == 3:
        return b64decode(msg + '=')
    elif len(msg)%4 == 2:
        print(msg)
        return b64decode(msg + '==')
    else:
        return b64decode(msg)

#------------------------------------------------------------------------
def fromBase32(msg):
    # Base32 decoding, we need to add the padding back
    # Add padding characters
    mod = len(msg) % 8
    if mod == 2:
        padding = "======"
    elif mod == 4:
        padding = "===="
    elif mod == 5:
        padding = "==="
    elif mod == 7:
        padding = "="
    else:
        padding = ""

    return b32decode(msg.upper() + padding)

            
#------------------------------------------------------------------------
def color(string, color=None):
    """
    Author: HarmJ0y, borrowed from Empire
    Change text color for the Linux terminal.
    """
    
    attr = []
    # bold
    attr.append('1')
    
    if color:
        if color.lower() == "red":
            attr.append('31')
        elif color.lower() == "green":
            attr.append('32')
        elif color.lower() == "blue":
            attr.append('34')
        return '\x1b[%sm%s\x1b[0m' % (';'.join(attr), string)

    else:
        if string.strip().startswith("[!]"):
            attr.append('31')
            return '\x1b[%sm%s\x1b[0m' % (';'.join(attr), string)
        elif string.strip().startswith("[+]"):
            attr.append('32')
            return '\x1b[%sm%s\x1b[0m' % (';'.join(attr), string)
        elif string.strip().startswith("[?]"):
            attr.append('33')
            return '\x1b[%sm%s\x1b[0m' % (';'.join(attr), string)
        elif string.strip().startswith("[*]"):
            attr.append('34')
            return '\x1b[%sm%s\x1b[0m' % (';'.join(attr), string)
        else:
            return string

#======================================================================================================
#                                            MAIN FUNCTION
#======================================================================================================
if __name__ == '__main__':
    password = 'pass'
    outputFileName = "out.zip"
    try:
        # Create and initialize the RC4 decryptor object
        rc4Decryptor = RC4(password)

        # Save data to a file
        print(color("[+] Decrypting using password [{}] and saving to output file [{}]".format(password, outputFileName)))
        with open(outputFileName, 'wb+') as fileHandle:
            fileHandle.write(rc4Decryptor.binaryDecrypt(bytearray(fromBase64URL(
                                                                  "EIu3wCinsPK_RDCVv5d-28e2TU-Ec1BHT83QblPN3mOo-L1-dXVkSPod7iTczcQ.tlULhh7p_QqO-k4FtUQ56nkbgIOkTNePAkkDmEWAggVL7hEcLJpKORiesVBGsol.AEJgjlMn2JczQo6KGqUAJ4GtnaXI3YZW7uEel8fq0kjjJvQfVhtbHHKyx9bEhJO.zxhc39atS4".replace('.','')))))
            fileHandle.close()
            print(color("[+] Output file [{}] saved successfully".format(outputFileName)))
    except IOError:
        print(color("[!] Could not write file [{}]".format(outputFileName)))
