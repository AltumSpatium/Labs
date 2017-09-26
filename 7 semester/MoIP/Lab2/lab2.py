import os
from des import DES
from tripledes import TripleDES
from magma import Magma
from magma import Modes

def main():
    while True:
        option = get_chosen_option()
        if option == '0':
            return
        if option in ('1', '2'):
            break
        else:
            print('Wrong input, try again...')

    print()

    while True:
        algorithm = get_chosen_algorithm()
        if algorithm in ('1', '2', '3'):
            break
        else:
            print('Wrong input, try again...')

    if algorithm == '1':
        key = get_key(8)
        method = DES(key)
    else:
        mode = get_mode(algorithm)
        if algorithm == '2':
            if mode == '1':
                print('Enter keys:')
                key = get_key(8)
                key2 = get_key(8)
                key3 = get_key(8)
                method = TripleDES(key, key2, key3)
            else:
                print('Enter keys:')
                key = get_key(8)
                key2 = get_key(8)
                method = TripleDES(key, key2)
        else:
            if mode == '1':
                key = get_key(32)
                method = Magma(key)
            elif mode == '2':
                key = get_key(32)
                method = Magma(key, mode=Modes.CBC)
            elif mode == '3':
                key = get_key(32)
                method = Magma(key, mode=Modes.OFB)
            else:
                key = get_key(32)
                method = Magma(key, mode=Modes.CFB)

    print()

    in_file_path = get_file_path('input')
    out_file_path = get_file_path('output')

    print()

    with open(in_file_path, 'rb') as f:
        data = f.read().decode()

    if option == '1':
        crypted_data = method.encrypt(data)
    else:
        crypted_data = method.decrypt(data)

    write_file(out_file_path, crypted_data)

    print('Done!')


def get_chosen_option():
    print('Choose an option:')
    print('1 -- Encrypt file')
    print('2 -- Decrypt file')
    print('0 -- Exit')
    option = input('>>')

    return option


def get_chosen_algorithm():
    print('Choose an algorithm:')
    print('1 -- Classic DES')
    print('2 -- Triple DES')
    print('3 -- ГОСТ 28147')
    algorithm = input('>>')

    return algorithm


def get_key(key_len):
    while True:
        print('Enter your key ({0} symbols):'.format(key_len))
        key = input('>>')

        if len(key) != key_len:
            print('Key must be {0} symbols long! Try again...'.format(key_len))
            continue

        return key


def get_mode(algorithm):
    while True:
        print('Choose mode:')
        if algorithm == '2':
            print('1 -- DES-EEE3')
            print('2 -- DES-EEE2')
        elif algorithm == '3':
            print('1 -- ECB')
            print('2 -- CBC')
            print('3 -- OFB')
            print('4 -- CFB')

        mode = input('>>')
        if mode not in ('1', '2', '3', '4'):
            print('Wrong input! Try again...')
        else:
            return mode


def get_file_path(kind):
    while True:
        print('Enter path to', kind, 'file:')
        path = os.path.abspath(input('>>'))
        if kind == 'input' and not os.path.isfile(path):
            print('No such file!')
            continue
        return path


def write_file(path, data):
    with open(path, 'wb') as f:
        f.write(data.encode())


if __name__ == '__main__':
    main()
