#!/usr/bin/env python3

import os
import os.path
import sys
import argparse

from util import check_challenge

from Cryptodome.Cipher import AES


def enc_text(fname, key):
    cipher = AES.new(key=key, mode=AES.MODE_ECB)

    pt = open(fname, 'r')

    # we want to emulate keyboard input --> send each byte separately
    ct = open(fname + '.enc', 'wb')
    while True:
        p = pt.read(1)
        if p == '':
            break
        p = bytes(p, 'ascii') + bytes(15)
        ct.write(cipher.encrypt(p))

    pt.close()
    ct.close()


def dec_text(fname, key):
    cipher = AES.new(key=key, mode=AES.MODE_ECB)

    ct = open(fname + '.enc', 'rb')
    pt = open(fname, 'w')

    while True:
        c = ct.read(16)
        if c == b'':
            break
        p = cipher.decrypt(c)
        p = p.decode('ascii')[0]
        pt.write(p)

    ct.close()
    pt.close()


def solve_challenge(blog_file, enc_file):
    with open(blog_file, 'r') as f:
        blog = f.read()

    with open(enc_file, 'rb') as f:
        enc = f.read()

    pw = ''
########################################################################
# enter your code here

    #get the sniffed text by padding the encrypted blog by 128 bits
    sniffed_text = [enc[s_letter:s_letter+16] for s_letter in range(0, len(enc), 16)]

    # get every letter of blog entry and store it in list
    letters = []
    for row in blog:
        for letter_of_blog in row:
            letters.append(letter_of_blog)

    #get most frequent element of blog (=space)
    most_frequent_element_blog = max(set(letters), key = letters.count)

   #get information of the length of the words in blog
    word_length_blog = []
    word_length_counter = 0

    for a in range(len(letters)):
        if letters[a] != most_frequent_element_blog:
            word_length_counter += 1
        else:
            word_length_blog.append(word_length_counter + 1)
            word_length_counter = 0

    #now we have to get the fist appearence of blog in sniffed text by 3 words -> we get startposition by letter variable
    for letter in range(len(sniffed_text)):
        if (letter + word_length_blog[1] + word_length_blog[2] + word_length_blog[3] + word_length_blog[4] + word_length_blog[5]) >= len(sniffed_text):
            break
        if sniffed_text[letter] == sniffed_text[letter + word_length_blog[1]]:
            if sniffed_text[letter + word_length_blog[1]] == sniffed_text[letter + word_length_blog[1] + word_length_blog[2]]:
                if sniffed_text[letter + word_length_blog[1] + word_length_blog[2]] == sniffed_text[letter + word_length_blog[1] + word_length_blog[2] + word_length_blog[3]]:
                            break

    #start of blog is correct offset
    start_of_blog = letter - word_length_blog[0] + 1    #1 = ' '
    end_of_blog = letter + len(blog)
    #the encrypted block text
    going_encrypted_block = sniffed_text[start_of_blog:end_of_blog]

    # Encrypt username
    username = "bruce"
    encrypted_username= []

    encryted_blog = {}
    for s in range(len(blog)):
        encryted_blog[blog[s]] = going_encrypted_block[s]

    for x in range(len(username)):
        encrypted_username.append(encryted_blog[username[x]])

    decrypted_blog = {}
    for t in range(len(blog)):
        decrypted_blog[going_encrypted_block[t]] = blog[t]

    # Start index of pw
    indexstart_pw = 0
    for y in range(len(sniffed_text)):
        if sniffed_text[y:y + len(encrypted_username)] == encrypted_username:
            indexstart_pw = y + len(encrypted_username)

    # Decode pw
    for c in range(len(sniffed_text)):
        n = indexstart_pw
        while n < start_of_blog:
            pw_decode = decrypted_blog[sniffed_text[n]]
            pw += pw_decode
            n += 1
        break

    ########################################################################
    with open('password', 'w') as f:
        f.write(pw)


def main():
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest='command', title='command')
    subparsers.required = True
    parser_e = subparsers.add_parser('e', help='encrypt')
    parser_e.add_argument('file', nargs='+')
    parser_d = subparsers.add_parser('d', help='decrypt')
    parser_d.add_argument('file', nargs='+')
    parser_g = subparsers.add_parser('g', help='keygen')
    parser_c = subparsers.add_parser('c', help='challenge')
    parser_c.add_argument(
        'blog',
        nargs='?',
        default='blog',
        help='default: blog')
    parser_c.add_argument(
        'sniffed_stream',
        nargs='?',
        default='sniffed_stream.enc',
        help='default: sniffed_stream.enc')
    args = parser.parse_args()

    if args.command == 'g':
        key = os.urandom(16)
        with open('key', 'wb') as f:
            f.write(key)
        return

    if args.command == 'e' or args.command == 'd':
        if not os.path.isfile('key'):
            print('no key found, run key generation first')
            return -1
        else:
            with open('key', 'rb') as f:
                key = f.read()
            files = [
                t for t in args.file if (
                    os.path.isfile(t) and not t == 'key')]

    if args.command == 'e':
        # we don't encrypt already encrypted files
        files = [t for t in files if not t.endswith('.enc')]
        if len(files) == 0:
            print('No valid files selected')
            return

        for f in files:
            enc_text(f, key)

        return

    if args.command == 'd':
        # we only want encrypted files
        files = [t[:-4] for t in files if t.endswith('.enc')]
        if len(files) == 0:
            print('No valid files selected')
            return

        for f in files:
            dec_text(f, key)

        return

    if args.command == 'c':
        solve_challenge(args.blog, args.sniffed_stream)

        check_challenge('password')

        return


if __name__ == "__main__":
    sys.exit(main())
