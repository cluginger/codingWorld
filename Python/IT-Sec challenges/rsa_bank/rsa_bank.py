import argparse
import json
from util import check_challenge
from Cryptodome.PublicKey import RSA


pkfile = 'rsa_key.pub'
skfile = 'rsa_key'


def generate():
    with open(skfile, 'rb') as f:
        sk = RSA.importKey(f.read(), 'PEM')

    transactions = {}
    for i in [2, 3, 5, 7]:
        transactions[i] = pow(i, sk.d, sk.n)

    with open("previous_transactions", "w") as f:
        json.dump(transactions, f, indent=2)

    with open("challenge_solution", "w") as f:
        f.write(str(pow(1000000, sk.d, sk.n)))


def rsa_kg():
    key = RSA.generate(4096)

    with open(skfile, 'wb') as f:
        f.write(key.exportKey('PEM'))

    pk = key.publickey()

    with open(pkfile, 'wb') as f:
        f.write(pk.exportKey('PEM'))


def solve_challenge(challenge_file, output_file):
    # get previous transactions
    with open(challenge_file, "r") as f:
        transactions = json.load(f)

    with open(pkfile, 'rb') as f:
        pk = RSA.importKey(f.read(), 'PEM')

    signature_one_million = -1

    ########################################################################
    # enter your code here


    signature_one_million = ((transactions['4'] ** 3) * (transactions['5'] ** 6)) % pk.n

    ########################################################################
    with open(output_file, 'w') as f:
        f.write(str(signature_one_million))


def verify_transaction(amount, signature):

    with open(pkfile, 'rb') as f:
        pk = RSA.importKey(f.read(), 'PEM')

    payout = pow(signature, pk.e, pk.n)

    if amount != 0 and payout == amount:
        print("Thank you. Here are your " + str(amount) + "€")
    else:
        print("This is not valid. Please do not try to cheat the bank")
    print("Thank you for you business with the national bank. Goodbye!")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest='command', title='command')
    subparsers.required = True
    parser_t = subparsers.add_parser('t', help='test')
    parser_s = subparsers.add_parser('s', help='solve')
    parser_c = subparsers.add_parser('c', help='check')
    parser_c.add_argument('challenge_file', nargs='?', default='previous_transactions',
                          help='default: previous_transactions')
    parser_c.add_argument('output_file', nargs='?',
                          default='challenge', help='default: challenge')
    parser_k = subparsers.add_parser('k', help='keygen')
    parser_g = subparsers.add_parser('g', help='generate transactions')

    args = parser.parse_args()

    print("         _._._                       _._._\n" +
          "        _|   |_                     _|   |_          \n" +
          "        | ... |_._._._._._._._._._._| ... |          \n" +
          "        | ||| |  o NATIONAL BANK o  | ||| |          \n" +
          "        | ~~~ |  ~~~    ~~~    ~~~  | ~~~ |          \n" +
          "   ())  |[-|-]| [-|-]  [-|-]  [-|-] |[-|-]|  ())     \n" +
          "  (())) |     |---------------------|     | (()))    \n" +
          " (())())| ~~~ |  ~~~    ~~~    ~~~  | ~~~ |(())())   \n" +
          " (()))()|[-|-]|  :::   .-*-.   :::  |[-|-]|(()))()   \n" +
          " ()))(()|     | |~|~|  |_|_|  |~|~| |     |()))(()   \n" +
          "    ||  |_____|_|_|_|__|_|_|__|_|_|_|_____|  ||      \n" +
          r" ~ ~^^ @@@@@@@@@@@@@@/=======\@@@@@@@@@@@@@@ ^^~ ~   \n" +
          "      ^~^~                                ~^~^")

    if args.command == 'g':
        generate()
    if args.command == 'k':
        rsa_kg()
    if args.command == 't':
        print("Welcome! Please enter how much you withdraw.\n" +
              " Now possible using our bank manager's signature!")
        withdraw = input("Withdraw this amount: ")
        sig = input("Using this signature: ")
        verify_transaction(int(withdraw), int(sig))
    if args.command == 'c':
        solve_challenge(args.challenge_file, args.output_file)
        check_challenge(args.output_file)
