import os
import json
from elliptic_curve_point import EllipticCurvePoint
from gost3410 import Gost3410


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

    params = load_params()
    if not params: return

    gost = Gost3410(params['a'], params['b'], params['p'],
        params['q'], params['x'], params['y'])

    if option == '1':
        generate_keys(gost)
        return

    in_file_path = get_file_path('input')
    with open(in_file_path, 'rb') as f:
        data = f.read().decode()

    print()

    keys = load_keys(params)
    if not keys: return

    if option == '2':
        d = keys[0]
        create_eds(in_file_path, gost, data, d)
    elif option == '3':
        Q = keys[1]
        verify_eds(in_file_path, gost, data, Q)


def get_chosen_option():
    print('Choose an option:')
    print('1 -- Generate keys')
    print('2 -- Create EDS')
    print('3 -- Verify EDS')
    print('0 -- Exit')
    option = input('>>')

    return option


def load_params():
    if not os.path.isfile('params.json'):
        print('File \'params.json\' does not exist!')
        return None

    with open('params.json', 'rb') as f:
        params = f.read().decode()
    return json.loads(params)


def generate_keys(gost):
    d, Q = gost.generate_keys()
    keys = {'private_key': d, 'public_key': {'x': Q.x, 'y': Q.y}}
    write_file('keys.json', json.dumps(keys))


def load_keys(params):
    if not os.path.isfile('keys.json'):
        print('File \'keys.json\' does not exist!')
        return None

    with open('keys.json', 'rb') as f:
        keys = f.read().decode()
    keys = json.loads(keys)
    d = keys['private_key']
    Q = EllipticCurvePoint(0, 0, params['a'], params['b'], params['p'])
    Q.x = keys['public_key']['x']
    Q.y = keys['public_key']['y']
    return d, Q


def create_eds(in_file_path, gost, data, d):
    sign = gost.sign(data, d)
    write_file(in_file_path + '_eds', sign)


def verify_eds(in_file_path, gost, data, Q):
    if not os.path.isfile(in_file_path + '_eds'):
        print('File \'{0}\' does not exist!'.format(in_file_path + '_eds'))
        return None

    with open(in_file_path + '_eds', 'rb') as f:
        eds = f.read()
    r = int.from_bytes(eds[:32], byteorder='big')
    s = int.from_bytes(eds[32:], byteorder='big')
    verified = gost.verify(data, Q, (r, s))
    print('EDS is {0}!'.format('verified' if verified else 'not verified'))


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
        f.write(data.encode() if isinstance(data, str) else data)


if __name__ == '__main__':
    main()
