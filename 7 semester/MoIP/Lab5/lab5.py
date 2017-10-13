import os
from md5 import md5

def main():
    while True:
        option = get_chosen_option()
        if option == '0':
            return
        if option is '1':
            break
        else:
            print('Wrong input, try again...')

    print()

    in_file_path = get_file_path('input')
    out_file_path = get_file_path('output')

    print()

    with open(in_file_path, 'rb') as f:
        data = f.read().decode()

    generated_hash = md5(data)

    write_file(out_file_path, generated_hash)

    print('Done!')


def get_chosen_option():
    print('Choose an option:')
    print('1 -- Generate hash')
    print('0 -- Exit')
    option = input('>>')

    return option


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
