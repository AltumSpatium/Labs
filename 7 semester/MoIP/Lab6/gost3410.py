import random
from gost3411 import GOST34112012
from elliptic_curve_point import EllipticCurvePoint

class Gost3410(object):
    def __init__(self, a, b, p, q, x, y):
        self.a = a
        self.b = b
        self.p = p

        # Point on elliptic curve, not equal to O
        self.P = EllipticCurvePoint(x, y, a, b, p)

        # The order of the group of points on elliptic curve
        self.q = q


    # Gost 34.11 hash function
    @staticmethod
    def hash_func(message):
        h = GOST34112012(data=message.encode(), digest_size=32)
        return h.digest()


    # Secret key: 0 < d < q; Public key: EC point Q = d * P
    def generate_keys(self):
        d = random.randint(1, self.q - 1)
        Q = d * self.P
        return d, Q


    @staticmethod
    def create_eds(r, s):
        r = r.to_bytes(32, byteorder='big')
        s = s.to_bytes(32, byteorder='big')
        return r + s


    def sign(self, message, private_key, k=0):
        digest = Gost3410.hash_func(message)
        a = int.from_bytes(digest, byteorder='big')
        e = a % self.q
        if e == 0: e = 1

        r, s = 0, 0
        while r == 0 or s == 0:
            if k == 0:
                k = random.randint(1, self.q - 1)
            C = k * self.P
            
            r = C.x % self.q
            if r == 0: continue

            s = (r * private_key + k * e) % self.q
        return Gost3410.create_eds(r, s)


    def verify(self, message, public_key, sign):
        r, s = sign
        digest = Gost3410.hash_func(message)
        a = int.from_bytes(digest, byteorder='big')
        e = a % self.q
        if e == 0: e = 1

        nu = EllipticCurvePoint.inverse_modulo(e, self.q)
        z1 = (s * nu) % self.q
        z2 = (-r * nu) % self.q
        C = z1 * self.P + z2 * public_key

        R = C.x % self.q
        return R == r
