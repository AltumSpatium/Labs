import os
import json
import rsa

def main():
    while True:
        option = get_chosen_option()
        if option == '0':
            return
        if option in ('1', '2', '3'):
            break
        else:
            print('Wrong input, try again...')

    print()

    if option == '3':
        generate_keys()
        return
    else:
        exponent, module = get_key()
        key = {'n': module}
        key['e' if option == '1' else 'd'] = exponent

    in_file_path = get_file_path('input')
    out_file_path = get_file_path('output')

    print()

    with open(in_file_path, 'rb') as f:
        data = f.read().decode()

    if option == '1':
        crypted_data = rsa.encrypt(data, key)
    else:
        crypted_data = rsa.decrypt(data, key)

    write_file(out_file_path, crypted_data)

    print('Done!')


def get_chosen_option():
    print('Choose an option:')
    print('1 -- Encrypt file')
    print('2 -- Decrypt file')
    print('3 -- Generate keys')
    print('0 -- Exit')
    option = input('>>')

    return option


def generate_keys():
    print('Enter key length (in bits):')
    bit_length = int(input('>>'))

    public_key, private_key = rsa.generate_keys(bit_length)
    print('Public key: e: {0}, n: {1}'.format(public_key['e'], public_key['n']))
    print('Private key: d: {0}, n: {1}'.format(private_key['d'], private_key['n']))
    keys = json.dumps(public_key) + '\n' + json.dumps(private_key)
    with open('keys.txt', 'w') as f:
        f.write(keys)


def get_key():
    print('Enter exponent:')
    exponent = int(input('>>'))

    print('Enter module:')
    module = int(input('>>'))

    return exponent, module


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
