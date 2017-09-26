BLOCK_SIZE = 8
PAD_SYMBOL = '0'
KEY_LENGTH = 8

# Initial permutation table
IP = [
    57, 49, 41, 33, 25, 17, 9,  1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7,
    56, 48, 40, 32, 24, 16, 8,  0,
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6
]

# Key permutation table 1
KP1 = [
    56, 48, 40, 32, 24, 16,  8,
    0, 57, 49, 41, 33, 25, 17,
    9,  1, 58, 50, 42, 34, 26,
    18, 10,  2, 59, 51, 43, 35,
    62, 54, 46, 38, 30, 22, 14,
    6, 61, 53, 45, 37, 29, 21,
    13,  5, 60, 52, 44, 36, 28,
    20, 12,  4, 27, 19, 11,  3
]

# Left rotations table
RT = [
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
]

# Key permutation table 2
KP2 = [
    13, 16, 10, 23,  0,  4,
    2, 27, 14,  5, 20,  9,
    22, 18, 11,  3, 25,  7,
    15,  6, 26, 19, 12,  1,
    40, 51, 30, 36, 46, 54,
    29, 39, 50, 44, 32, 47,
    43, 48, 38, 55, 33, 52,
    45, 41, 49, 35, 28, 31
]

# Expansion table
ET = [
    31,  0,  1,  2,  3,  4,
    3,  4,  5,  6,  7,  8,
    7,  8,  9, 10, 11, 12,
    11, 12, 13, 14, 15, 16,
    15, 16, 17, 18, 19, 20,
    19, 20, 21, 22, 23, 24,
    23, 24, 25, 26, 27, 28,
    27, 28, 29, 30, 31,  0
]

# The S-boxes
SBOX = [
    # S1
    [
        [14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7],
        [0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8],
        [4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0],
        [15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13]
    ],

    # S2
    [
        [15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10],
        [3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5],
        [0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15],
        [13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9]
    ],

    # S3
    [
        [10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8],
        [13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1],
        [13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7],
        [1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12]
    ],

    # S4
    [
        [7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15],
        [13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9],
        [10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4],
        [3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14]
    ],

    # S5
    [
        [2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9],
        [14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6],
        [4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14],
        [11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3]
    ],

    # S6
    [
        [12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11],
        [10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8],
        [9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6],
        [4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13]
    ],

    # S7
    [
        [4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1],
        [13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6],
        [1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2],
        [6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12]
    ],

    # S8
    [
        [13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7],
        [1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2],
        [7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8],
        [2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11]
    ]
]

# P-permutation table
P = [
    15, 6, 19, 20, 28, 11, 27, 16,
    0, 14, 22, 25, 4, 17, 30, 9,
    1, 7, 23, 13, 31, 26, 2, 8,
    18, 12, 29, 5, 21, 10, 3, 24
]

# Final permutation table
FP = [
    39,  7, 47, 15, 55, 23, 63, 31,
    38,  6, 46, 14, 54, 22, 62, 30,
    37,  5, 45, 13, 53, 21, 61, 29,
    36,  4, 44, 12, 52, 20, 60, 28,
    35,  3, 43, 11, 51, 19, 59, 27,
    34,  2, 42, 10, 50, 18, 58, 26,
    33,  1, 41,  9, 49, 17, 57, 25,
    32,  0, 40,  8, 48, 16, 56, 24
]


class DES(object):
    def __init__(self, key):
        if len(key) != KEY_LENGTH:
            raise ValueError('Key size must be {0} bytes!'.format(KEY_LENGTH))
        self.key = key
        self.keys = []

        self.generate_keys()


    def string_to_bitlist(self, string):
        bitlist = []
        for char in string:
            bits = bin(ord(char))[2:]
            bits = '0' * (8 - len(bits)) + bits
            bitlist.extend(map(int, bits))
        return bitlist


    def bitlist_to_string(self, bitlist):
        chars = []
        for i in range(len(bitlist) // 8):
            byte = bitlist[i*8:(i+1)*8]
            byte_str = ''.join(map(str, byte))

            chars.append(chr(int(byte_str, 2)))
        return ''.join(chars)


    def permutate(self, block, table):
        return list(map(lambda x: block[x], table))


    def initial_permutation(self, block):
        return self.permutate(block, IP)


    def final_permutation(self, block):
        return self.permutate(block, FP)


    def prepare_data(self, data):
        if len(data) % BLOCK_SIZE != 0:
            data += PAD_SYMBOL * (BLOCK_SIZE - len(data) % BLOCK_SIZE)
        return data


    def prepare_result(self, result):
        return result[:-BLOCK_SIZE] + result[-BLOCK_SIZE:].rstrip(PAD_SYMBOL)


    def generate_keys(self):
        def rotate(l, n):
            return l[n:] + l[:n]

        key = self.permutate(self.string_to_bitlist(self.key), KP1)
        C = key[:28]
        D = key[28:]

        for i in range(16):
            rotations_count = RT[i]
            C = rotate(C, rotations_count)
            D = rotate(D, rotations_count)

            new_key = self.permutate(C + D, KP2)
            self.keys.append(new_key)


    def expansion_func(self, R):
        return self.permutate(R, ET)


    def S_transformation(self, R):
        get_bin = lambda x: '{0:04b}'.format(x)

        B = [R[i:i+6] for i in range(0, len(R), 6)]
        B_ = []

        for i in range(8):
            Bi = B[i]
            Si = SBOX[i]

            a = int(str(Bi[0]) + str(Bi[5]), 2)
            b = int(''.join(map(str, Bi[1:5])), 2)

            B_i = Si[a][b]
            B_i = list(map(int, list(get_bin(B_i))))
            B_.extend(B_i)

        return B_


    def p_permutation(self, B_):
        return self.permutate(B_, P)


    def feistel_func(self, R, key):
        R = self.expansion_func(R)
        R = list(map(lambda x, y: x ^ y, R, key))
        B_ = self.S_transformation(R)
        return self.p_permutation(B_)


    def crypt_block(self, block, decrypt):
        block = self.initial_permutation(block)
        L = block[:32]
        R = block[32:]

        i = 15 if decrypt else 0
        i_adj = -1 if decrypt else 1

        for _ in range(16):
            next_L = R[:]
            f = self.feistel_func(R, self.keys[i])
            next_R = list(map(lambda x, y: x ^ y, L, f))

            L = next_L
            R = next_R

            i += i_adj

        return self.final_permutation(R + L)
        

    def crypt(self, data, decrypt=False):
        blocks = [data[i:i+BLOCK_SIZE] for i in range(0, len(data), BLOCK_SIZE)]
        crypted_data = []

        for block in blocks:
            block = self.string_to_bitlist(block)
            crypted_block = self.crypt_block(block, decrypt)
            crypted_data.append(self.bitlist_to_string(crypted_block))

        return ''.join(crypted_data)
        

    def encrypt(self, data):
        data = self.prepare_data(data)
        return self.crypt(data)


    def decrypt(self, data):
        result = self.crypt(data, decrypt=True)
        return self.prepare_result(result)
