#!/usr/bin/env python3

import os
import os.path
import sys
import struct
import argparse
import time

from util import check_challenge

from csprng import csprng_bytes, csprng_seed
from Cryptodome.Cipher import AES
from Cryptodome.Util.Padding import pad, unpad
from Cryptodome.Hash import SHA512
from Cryptodome.Cipher import ChaCha20

def enc_file(fname):
    iv = csprng_bytes(16)
    key = csprng_bytes(16)

    cipher = AES.new(key=key, mode=AES.MODE_CBC, iv=iv)
    f = open(fname, 'rb')
    padded_pt = pad(f.read(), AES.block_size, style='pkcs7')
    ct = cipher.encrypt(padded_pt)
    f.close()

    with open(fname + '.enc', 'wb') as f:
        f.write(iv)
        f.write(ct)

    with open(fname + '.key', 'wb') as f:
        f.write(key)


def dec_file(fname):
    with open(fname + '.key', 'rb') as f:
        key = f.read()

    with open(fname + '.enc', 'rb') as f:
        iv = f.read(16)
        ct = f.read()

    cipher = AES.new(key=key, mode=AES.MODE_CBC, iv=iv)
    padded_pt = cipher.decrypt(ct)

    try:
        pt = unpad(padded_pt, AES.block_size, style='pkcs7')
    except ValueError:
        print(fname + ': decryption failed, invalid padding')

    with open(fname, 'wb') as f:
        f.write(pt)


def solve_challenge(fname):
    with open(fname + '.enc', 'rb') as f:
        iv = f.read(16)
        ct = f.read()

    fname_enc = fname + '.enc'

    key = bytes(16)
########################################################################
# enter your code here
    mtime = int(os.path.getmtime(fname_enc))
    pidMax = 80000
    endLoop = False

    for pid in range(pidMax):

        for offsetTime in range(-1, 1):
            seed_bytes = struct.pack('<Qi', mtime + offsetTime, pid)

            digest = SHA512.new()
            digest.update(seed_bytes)
            d = digest.digest()

            keyBrute = d[:32]
            ivBrute = d[32:40]

            chacha = ChaCha20.new(key=keyBrute, nonce=ivBrute)

            new_iv = chacha.encrypt(b'\0' * 16)
            key = chacha.encrypt(b'\0' * 16)

            if iv == new_iv:
                endLoop = True
                break
            
        if endLoop:
            break
########################################################################

    cipher = AES.new(key=key, mode=AES.MODE_CBC, iv=iv)
    padded_pt = cipher.decrypt(ct)

    try:
        pt = unpad(padded_pt, AES.block_size, style='pkcs7')
    except ValueError:
        print(fname + ': decryption failed, invalid padding')
        return

    with open(fname, 'wb') as f:
        f.write(pt)


def main():
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest='command', title='command')
    subparsers.required = True
    parser_e = subparsers.add_parser('e', help='encrypt')
    parser_e.add_argument('file', nargs='+')
    parser_d = subparsers.add_parser('d', help='decrypt')
    parser_d.add_argument('file', nargs='+')
    parser_c = subparsers.add_parser('c', help='challenge')
    parser_c.add_argument(
        'file',
        nargs='*',
        default=['challenge.enc'],
        help='default: challenge.enc')
    args = parser.parse_args()

    files = [
        t for t in args.file if (
            os.path.isfile(t) and not t.endswith('.key'))]

    # seed the csprng
    t = int(time.time())  # this is similar to calling t = time(0) in C
    t = struct.pack('<Qi', t, os.getpid())
    csprng_seed(t)

    if args.command == 'e':
        # we don't encrypt already encrypted files
        files = [t for t in files if not t.endswith('.enc')]
        if len(files) == 0:
            print('No valid files selected')
            return
        for f in files:
            enc_file(f)
        return

    if args.command == 'd':
        # we only want encrypted files
        files = [t[:-4] for t in files if t.endswith('.enc')]
        if len(files) == 0:
            print('No valid files selected')
            return
        for f in files:
            dec_file(f)
        return

    # challenge: decrypt without having the key
    if args.command == 'c':
        # we only want encrypted files
        files = [t[:-4] for t in files if t.endswith('.enc')]
        if len(files) == 0:
            print('No valid files selected')
            return

        for f in files:
            solve_challenge(f)
            check_challenge(f)
        return


if __name__ == "__main__":
    sys.exit(main())
