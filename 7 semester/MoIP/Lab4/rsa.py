import random


def is_prime(n):
    i = 2
    while (i * i <= n):
        if (n % i == 0):
            return False
        i += 1
    return True


def are_relatively_prime(a, b):
    for n in range(2, min(a, b) + 1):
        if a % n == b % n == 0:
            return False
    return True


def generate_keys(bit_length):
    if bit_length < 8:
        raise ValueError('Key bit length must be above 8!')

    min_n = 1 << (bit_length - 1)
    max_n = (1 << bit_length) - 1

    min_prime = 1 << (bit_length // 2 - 1)
    max_prime = 1 << (bit_length // 2 + 1)

    primes = [i for i in range(min_prime, max_prime) if is_prime(i)]

    p = random.choice(primes)
    primes.remove(p)

    for q in primes:
        if min_n <= p * q <= max_n:
            break

    n = p * q
    euler = (p - 1) * (q - 1)

    for e in range(3, euler, 2):
        if are_relatively_prime(e, euler):
            break

    for d in range(3, euler, 2):
        if d * e % euler == 1:
            break

    return ({'n': n, 'e': e}, {'n': n, 'd': d})


def prepare_data(data):
    return list(map(ord, list(data)))


def prepare_result(result):
    return ''.join(list(map(chr, result)))


def crypt(data, exponent, module):
    data = prepare_data(data)
    result = [pow(sym, exponent, module) for sym in data]
    return prepare_result(result)


def encrypt(data, public_key):
    return crypt(data, public_key['e'], public_key['n'])


def decrypt(data, private_key):
    return crypt(data, private_key['d'], private_key['n'])
