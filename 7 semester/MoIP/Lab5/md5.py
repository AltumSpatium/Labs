import math
 
rotate_count = [
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
]
 
T = [int(abs(math.sin(i+1)) * 2**32) & 0xFFFFFFFF for i in range(64)]

A = 0x67452301
B = 0xefcdab89
C = 0x98badcfe
D = 0x10325476
 
functions = [lambda b, c, d: (b & c) | (~b & d)] * 16 + \
    [lambda b, c, d: (d & b) | (~d & c)] * 16 + \
    [lambda b, c, d: b ^ c ^ d] * 16 + \
    [lambda b, c, d: c ^ (b | ~d)] * 16
 
index_functions = [lambda i: i] * 16 + \
    [lambda i: (5*i + 1) % 16] * 16 + \
    [lambda i: (3*i + 5) % 16] * 16 + \
    [lambda i: (7*i) % 16] * 16
 

def to_bytearray(data):
    data_bytes = bytearray(data)
    length = (8 * len(data_bytes)) & 0xffffffffffffffff
    data_bytes.append(0x80)
    while len(data_bytes) % 64 != 56:
        data_bytes.append(0)
    data_bytes += length.to_bytes(8, byteorder='little')   
    return data_bytes


def left_rotate(num, bit_count):
    num &= 0xFFFFFFFF
    return ((num << bit_count) | (num >> (32 - bit_count))) & 0xFFFFFFFF
 

def parts(data, length):
    return [data[i: i + length] for i in range(0, len(data), length)]


def md5(data):
    global A, B, C, D

    data = to_bytearray(data.encode()) 
    for part in parts(data, 64):
        a, b, c, d = A, B, C, D

        for i in range(64):
            f = functions[i](b, c, d)
            index = index_functions[i](i)

            x_k = int.from_bytes(part[index * 4: index * 4 + 4], byteorder='little')
            a = (b + left_rotate(a + f + T[i] + x_k, rotate_count[i])) & 0xFFFFFFFF
            a, b, c, d = d, a, b, c

        A = A + a & 0xFFFFFFFF
        B = B + b & 0xFFFFFFFF
        C = C + c & 0xFFFFFFFF
        D = D + d & 0xFFFFFFFF

    result = sum(x << (32 * i) for i, x in enumerate([A, B, C, D]))
    hex_hash = result.to_bytes(16, byteorder='little')
    return '{:032x}'.format(int.from_bytes(hex_hash, byteorder='big'))
