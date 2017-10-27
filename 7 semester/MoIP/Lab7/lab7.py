import os
import json
from elliptic_curve_point import EllipticCurvePoint
from eccrypt import ECCrypt


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

    crypter = ECCrypt(params['a'], params['b'], params['p'],
        params['q'], params['x'], params['y'])

    if option == '1':
        generate_keys(crypter)
        return

    in_file_path = get_file_path('input')
    out_file_path = get_file_path('output')
    with open(in_file_path, 'rb') as f:
        data = f.read()

    print()

    keys = load_keys(params)
    if not keys: return

    if option == '2':
        Q = keys[1]
        encrypt(out_file_path, crypter, data.decode(), Q)
    elif option == '3':
        d = keys[0]
        decrypt(out_file_path, crypter, data, d, params)


def get_chosen_option():
    print('Choose an option:')
    print('1 -- Generate keys')
    print('2 -- Encrypt')
    print('3 -- Decrypt')
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


def generate_keys(crypter):
    d, Q = crypter.generate_keys()
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


def encrypt(out_file_path, crypter, data, Q):
    points = crypter.encrypt(data, Q)
    crypted_data = b''
    for C1, C2 in points:
        crypted_data += C1.x.to_bytes(32, byteorder='big')
        crypted_data += C1.y.to_bytes(32, byteorder='big')
        crypted_data += C2.x.to_bytes(32, byteorder='big')
        crypted_data += C2.y.to_bytes(32, byteorder='big')
    write_file(out_file_path, crypted_data)


def decrypt(out_file_path, crypter, data, d, params):
    points_bytes = [data[i:i+128] for i in range(0, len(data), 128)]
    result = ''
    for point in points_bytes:
        x1 = int.from_bytes(point[:32], byteorder='big')
        y1 = int.from_bytes(point[32:64], byteorder='big')
        x2 = int.from_bytes(point[64:96], byteorder='big')
        y2 = int.from_bytes(point[96:], byteorder='big')
        C1 = EllipticCurvePoint(x1, y1, params['a'], params['b'], params['p'])
        C2 = EllipticCurvePoint(x2, y2, params['a'], params['b'], params['p'])
        result += crypter.decrypt(d, (C1, C2))
    write_file(out_file_path, result)


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
