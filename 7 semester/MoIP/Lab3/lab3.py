import os
import aes


def main():
    while True:
        option = get_chosen_option()
        if option == "0":
            return
        if option in ('1', '2'):
            break
        else:
            print("Wrong input, try again...")

    print()

    in_file_path = get_file_path("input")
    out_file_path = get_file_path("output")

    print()

    key = get_secret_key()

    with open(in_file_path, 'rb') as f:
        data = f.read()

    if option == '1':
        crypted_data = get_crypted_data(data, key)
    else:
        crypted_data = get_crypted_data(data, key, decrypt=True)

    write_file(out_file_path, crypted_data)


def get_chosen_option():
    print("Choose an option:")
    print("1 -- Encrypt file")
    print("2 -- Decrypt file")
    print("0 -- Exit")
    option = input(">>")

    return option


def get_file_path(kind):
    while True:
        print("Enter path to", kind, "file:")
        path = os.path.abspath(input(">>"))
        if kind == "input" and not os.path.isfile(path):
            print("No such file!")
            continue
        return path


def get_secret_key():
    while True:
        print("Enter your key (less than 16 symbols):")
        key = input(">>")

        if len(key) > 16:
            print("The key is too long! Try again...")
            continue

        return key


def get_crypted_data(data, key, decrypt=False):
    if decrypt == True:
        operation = aes.decrypt
    else:
        operation = aes.encrypt

    crypted_data = []
    temp = []
    for byte in data:
        temp.append(byte)
        if (len(temp) == 16):
            crypted_part = operation(temp, key)
            crypted_data.extend(crypted_part)
            del temp[:]
    else:
        if 0 < len(temp) < 16:
            empty_spaces = 16 - len(temp)
            for i in range(empty_spaces - 1):
                temp.append(0)
            temp.append(1)
            crypted_part = operation(temp, key)
            crypted_data.extend(crypted_part)

    return crypted_data


def write_file(path, data):
    with open(path, 'xb') as ff:
        ff.write(bytes(data))


if __name__ == "__main__":
    main()
