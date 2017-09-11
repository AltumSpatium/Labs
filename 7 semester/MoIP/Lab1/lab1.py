import os
import caesar


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

    shift_size = get_shift_size()

    print()

    alphabet = get_alphabet()

    with open(in_file_path, 'r') as f:
        data = f.read()

    if option == '1':
        crypted_data = caesar.encrypt(data, shift_size, alphabet)
    else:
        crypted_data = caesar.decrypt(data, shift_size, alphabet)

    write_file(out_file_path, crypted_data)

    print("Done!")


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


def get_shift_size():
    while True:
        print("Enter shift size:")
        shift_size = input(">>")
        if shift_size.isnumeric():
            return int(shift_size)
        print("Shift size must be a number!")
        continue


def get_alphabet():
    while True:
        print("Choose an alphabet:")
        print("1 -- russian")
        print("2 -- english")
        option = input(">>")

        alphabet = 'ru' if option == '1' else 'en'
        return alphabet


def write_file(path, data):
    with open(path, 'w') as f:
        f.write(data)


if __name__ == "__main__":
    main()
