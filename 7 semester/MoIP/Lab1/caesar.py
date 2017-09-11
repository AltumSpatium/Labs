alphabets = {
    'ru': 'абвгдеёжзийклмнопрстуфхцчшщъыьэюя',
    'en': 'abcdefghijklmnopqrstuvwxyz'
}


def crypt(data, shift_size, alphabet, decrypt=False):
    result = ''
    shift_direction = -1 if decrypt else 1
    for symbol in data:
        sym_index = alphabet.index(symbol.lower()) if symbol.lower() in alphabet else -1
        new_index = sym_index + shift_size * shift_direction

        new_symbol = symbol if sym_index == -1 else alphabet[new_index % len(alphabet)]
        result += new_symbol.upper() if symbol.isupper() else new_symbol
    return result


def encrypt(data, shift_size, alphabet):
    return crypt(data, shift_size, alphabets[alphabet])


def decrypt(data, shift_size, alphabet):
    return crypt(data, shift_size, alphabets[alphabet], decrypt=True)
