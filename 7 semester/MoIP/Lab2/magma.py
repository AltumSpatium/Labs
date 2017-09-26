from enum import Enum

BLOCK_SIZE = 8
PAD_SYMBOL = '0'
KEY_LENGTH = 32

SBOX = [
    [4, 10, 9, 2, 13, 8, 0, 14, 6, 11, 1, 12, 7, 15, 5, 3],
    [14, 11, 4, 12, 6, 13, 15, 10, 2, 3, 8, 1, 0, 7, 5, 9],
    [5, 8, 1, 13, 10, 3, 4, 2, 14, 15, 12, 7, 6, 0, 9, 11],
    [7, 13, 10, 1, 0, 8, 9, 15, 14, 4, 6, 12, 11, 2, 5, 3],
    [6, 12, 7, 1, 5, 15, 13, 8, 4, 10, 9, 14, 0, 3, 11, 2],
    [4, 11, 10, 0, 7, 2, 1, 13, 3, 6, 8, 5, 9, 12, 15, 14],
    [13, 11, 4, 1, 3, 15, 5, 9, 0, 10, 14, 7, 6, 8, 2, 12],
    [1, 15, 13, 0, 5, 7, 10, 4, 9, 2, 3, 14, 6, 11, 8, 12]
]


class Modes(Enum):
    ECB = 0
    CBC = 1
    OFB = 2
    CFB = 3


class Magma(object):
    def __init__(self, key, mode=Modes.ECB, iv=8 * '0'):
        if len(key) != KEY_LENGTH:
            raise ValueError('Key size must be {0} bytes!'.format(KEY_LENGTH))
        if mode in (Modes.CBC, Modes.OFB, Modes.CFB) and iv is None:
            raise ValueError('IV must be provided when using CBC/OFB/CFB modes!')

        self.key = key
        self.keys = []
        self.mode = mode
        self.rounds_count = 32
        self.last_result = None
        self.gamma = []
        self.iv = iv

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


    def prepare_data(self, data):
        if len(data) % BLOCK_SIZE != 0:
            data += PAD_SYMBOL * (BLOCK_SIZE - len(data) % BLOCK_SIZE)
        return data


    def prepare_result(self, result):
        return result[:-BLOCK_SIZE] + result[-BLOCK_SIZE:].rstrip(PAD_SYMBOL)


    def synchromail_crypt(self):
        iv = self.prepare_data(self.iv)
        iv = self.string_to_bitlist(iv)
        return self.ecb_crypt(iv, False)


    def generate_gamma(self, blocks_count):
        get_bin = lambda x: '{0:032b}'.format(x)

        C1 = 16843012
        C2 = 16843009

        sync = self.synchromail_crypt()
        N4 = sync[:32]
        N3 = sync[32:]

        for i in range(blocks_count):
            N3 = int(''.join(map(str, N3)), 2)
            N3 = (N3 + C2) % 2 ** 32
            N3 = list(map(int, list(get_bin(N3))))

            N4 = int(''.join(map(str, N4)), 2)
            N4 = (N4 + C1) % 2 ** 32
            N4 = list(map(int, list(get_bin(N4))))

            self.gamma.append(self.ecb_crypt(N3 + N4, False))


    def generate_keys(self):
        key = self.string_to_bitlist(self.key)
        keys = [key[i:i+32] for i in range(0, len(key), 32)]
        self.keys.extend(keys * 3)
        self.keys.extend(reversed(keys))


    def F(self, R, key):
        rotate = lambda l, n: l[n:] + l[:n]
        get_bin = lambda x: '{0:032b}'.format(x)
        A_ = []

        r = int(''.join(map(str, R)), 2)
        k = int(''.join(map(str, key)), 2)
        A = (r + k) % 2 ** 32
        A = list(map(int, list(get_bin(A))))
        A = [A[i:i+4] for i in range(0, len(A), 4)]

        for i in range(len(A)):
            a = int(''.join(map(str, A[i])), 2)
            a_ = SBOX[i][a]
            a_ = list(map(int, list(get_bin(a_))))
            A_.extend(a_)

        return rotate(A_, 11)


    def ecb_crypt(self, block, decrypt):
        L = block[:32]
        R = block[32:]

        i = 31 if decrypt else 0
        i_adj = -1 if decrypt else 1

        for _ in range(self.rounds_count):
            next_L = R[:]
            f = self.F(R, self.keys[i])
            next_R = list(map(lambda x, y: x ^ y, L, f))

            L = next_L
            R = next_R

            i += i_adj

        return R + L


    def crypt_block(self, block, decrypt, i):
        if self.mode == Modes.ECB:
            return self.ecb_crypt(block, decrypt)
        elif self.mode == Modes.CBC:
            if decrypt:
                result = self.ecb_crypt(block, decrypt)
                if i == 0:
                    iv = self.prepare_data(self.iv)
                    iv = self.string_to_bitlist(iv)
                    result = list(map(lambda x, y: x ^ y, iv, result))
                else:
                    result = list(map(lambda x, y: x ^ y, self.last_result, result))
                self.last_result = block
            else:
                if i == 0:
                    iv = self.prepare_data(self.iv)
                    iv = self.string_to_bitlist(iv)
                    block = list(map(lambda x, y: x ^ y, block, iv))
                else:
                    block = list(map(lambda x, y: x ^ y, block, self.last_result))
                result = self.ecb_crypt(block, decrypt)
                self.last_result = result[:]
            return result
            
        elif self.mode == Modes.OFB:
            return list(map(lambda x, y: x ^ y, block, self.gamma[i]))
        elif self.mode == Modes.CFB:
            result = list(map(lambda x, y: x ^ y, block, self.gamma[i]))
            if decrypt:
                self.gamma.append(block)
            else:
                self.gamma.append(result)
            return result


    def crypt(self, data, decrypt=False):
        blocks = [data[i:i+BLOCK_SIZE] for i in range(0, len(data), BLOCK_SIZE)]
        crypted_data = []
        if self.mode == Modes.OFB:
            self.generate_gamma(len(blocks))
        elif self.mode == Modes.CFB:
            self.gamma = [self.synchromail_crypt()]

        for i in range(len(blocks)):
            block = self.string_to_bitlist(blocks[i])
            crypted_block = self.crypt_block(block, decrypt, i)
            crypted_data.append(self.bitlist_to_string(crypted_block))

        return ''.join(crypted_data)


    def encrypt(self, data):
        data = self.prepare_data(data)
        return self.crypt(data)


    def decrypt(self, data):
        result = self.crypt(data, decrypt=True)
        return self.prepare_result(result)
