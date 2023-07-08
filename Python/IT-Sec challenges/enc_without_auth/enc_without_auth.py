#!/usr/bin/python3
import sys
import socket
import json
import base64
import os
import os.path
import argparse

from Cryptodome.Util.Padding import pad, unpad
from util import check_challenge
from utils import State, encrypt, decrypt
from Cryptodome.Random import get_random_bytes
HOST = '0.0.0.0'
highport = 6667
lowport = 6666

aes_block_length = 16
id_len = 8


def readTranscript(fname):
    with open(fname, "r") as f:
        ts = json.load(f)

    return [[base64.b64decode(packet) for packet in record] for record in ts]


def openConnection(high):
    if high:
        port = highport
    else:
        port = lowport

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, port))
    return s


def leakID():
    with open("configs/highCard.json", 'r') as f:
        config = json.load(f)

    return base64.b64decode(config["id"])


def sendAndWaitReply(socket, packet):
    socket.sendall(packet)
    return socket.recv(1024)


def solve_challenge(fname):

    # read sniffed low-security transcript between card and scanner
    sniffed = readTranscript(fname)

    # open connections to both scanners
    shigh = openConnection(True)
    slow = openConnection(False)

    # read in the id of high-security card ("database leak")
    cardID = leakID()

########################################################################
# enter your code here
    padded_cardID = pad(cardID, aes_block_length)

    # iv + id + challenge (highcard)
    slow.sendall(bytes(sniffed[0][0])[:16] + bytes(sniffed[0][0])[16:32] + padded_cardID)
    res = slow.recv(48)
    iv = res[16:32]

    # reset slow
    slow.sendall(sniffed[0][2])

    # iv + encoded highcard + random challenge
    shigh.sendall(iv + res[32:48] + get_random_bytes(16))
    res = shigh.recv(80)
    ivHigh = res[:16]
    idsHigh = res[16:32]

    padded_cmd = pad("opendoor".encode("ascii"), aes_block_length)
    slow = openConnection(False)
    # iv + id + challenge (response from high) + cmd
    slow.sendall(bytes(sniffed[0][0])[:16] + bytes(sniffed[0][0])[16:32] + res[64:80] + padded_cmd)
    res = slow.recv(80)

    # ivHigh + idsHigh + iv_challenge + enc_challenge
    shigh.sendall(ivHigh + idsHigh + res[16:64])
########################################################################


def main():
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest='command', title='command')
    subparsers.required = True
    parser_c = subparsers.add_parser('c', help='challenge')
    parser_c.add_argument('sniffed_transcript', nargs='?',
                          default='sniffed.json', help='default: sniffed.json')
    args = parser.parse_args()

    if args.command == 'c':

        if os.path.isfile('highdoor'):
            os.remove('highdoor')

        solve_challenge(args.sniffed_transcript)
        check_challenge('highdoor')
        return


if __name__ == "__main__":
    sys.exit(main())
