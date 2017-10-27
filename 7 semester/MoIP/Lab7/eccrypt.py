import random
from elliptic_curve_point import EllipticCurvePoint


class ECCrypt(object):
    def __init__(self, a, b, p, q, x, y):
        self.a = a
        self.b = b
        self.p = p

        self.P = EllipticCurvePoint(x, y, a, b, p)
        self.q = q


    def generate_keys(self):
        d = random.randint(1, self.q - 1)
        Q = d * self.P
        return d, Q


    def prepare_data(self, message):
        return int.from_bytes(message.encode(), byteorder='little')


    def prepare_result(self, result):
        return result.to_bytes(8, byteorder='little').decode()


    def __encrypt(self, message, Q):
        y = message // 2
        x = message - y
        M = EllipticCurvePoint(x, y, self.a, self.b, self.p)

        k = random.randint(1, self.q - 1)
        C1 = k * self.P
        C2 = M + k * Q
        return C1, C2


    def decrypt(self, d, cipher):
        C1, C2 = cipher
        M = C2 - d * C1
        return self.prepare_result(M.x + M.y)


    def encrypt(self, message, Q):
        blocks = [self.prepare_data(message[i:i+8]) for i in range(0, len(message), 8)]
        result = list(map(lambda block: self.__encrypt(block, Q), blocks))
        return result
