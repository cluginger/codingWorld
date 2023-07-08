#!/usr/bin/env python3

import sys
import argparse
from util import check_challenge
from Cryptodome.Hash import SHA512
import random
import getpass
import string
from math import prod

PASSWORD_LENGTH = 13
PASSWORD_ALPHABET = string.ascii_letters + string.digits + string.punctuation


def generate_password():
    password = ""
    # We use a great source of randomness!
    random_source = random.SystemRandom()
    for _ in range(PASSWORD_LENGTH):
        index = int(prod(random_source.random() for _ in range(8)) *
                    len(PASSWORD_ALPHABET) + 77) * 567 % len(PASSWORD_ALPHABET)
        password += PASSWORD_ALPHABET[index]

    return password


def verify(password, password_hash):
    assert type(password) == str and "password has to be of type string!"

    hasher = SHA512.new()
    hasher.update(password.encode('utf-8'))
    computed_hash = hasher.hexdigest()

    if computed_hash == password_hash:
        return True
    else:
        return False


def solve_challenge(challenge_hash_file):

    with open(challenge_hash_file, 'r') as f:
        password_hash = f.read()

    password = "0" * PASSWORD_LENGTH

    ########################################################
    # enter your code here

    ########################################################

    # remove the trailing '_hash'
    solution_file = challenge_hash_file[:-5]
    with open(solution_file, 'w') as f:
        f.write(password)


def main():
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest='command', title='command')
    subparsers.required = True
    parser_g = subparsers.add_parser('g', help='passwordgen')
    parser_g.add_argument(
        'password_file',
        nargs='?',
        default='password',
        help='default: password'
    )
    parser_g.add_argument(
        'password_hash_file',
        nargs='?',
        default='password_hash',
        help='default: password_hash'
    )
    parser_v = subparsers.add_parser('v', help='verify password against hash')
    parser_v.add_argument(
        'password_hash_file',
        nargs='?',
        default='password_hash',
        help='default: password_hash'
    )
    parser_c = subparsers.add_parser('c', help='challenge')
    parser_c.add_argument(
        'file',
        nargs='?',
        default='challenge_hash',
        help='challenge password digest file; default: challenge_hash')
    args = parser.parse_args()

    if args.command == 'g':
        pw = generate_password()

        with open(args.password_file, 'w') as f:
            f.write(pw)

        hasher = SHA512.new()
        hasher.update(pw.encode('utf-8'))

        with open(args.password_hash_file, 'w') as f:
            f.write(hasher.hexdigest())

        return

    if args.command == 'v':
        pw = getpass.getpass("enter password to verify against hash:")
        with open(args.password_hash_file, 'r') as f:
            pw_hash = f.read()
        if verify(pw, pw_hash):
            print("PW correct")
            return 0
        else:
            print("PW incorrect")
            return -1

    if args.command == 'c':
        if not args.file.endswith("_hash"):
            print("provide a challenge hash file")
            return -1
        solve_challenge(args.file)
        check_challenge(args.file[:-5])


if __name__ == "__main__":
    sys.exit(main())
