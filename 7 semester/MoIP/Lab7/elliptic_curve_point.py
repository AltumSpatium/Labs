class EllipticCurvePoint(object):
    def __init__(self, x=0, y=0, a=0, b=0, p=0):
        # (x, y) coordinates of a point on elliptic curve
        self.x = x
        self.y = y

        # Coefficients of elliptic curve
        self.a = a
        self.b = b

        # Elliptic curve modulo
        self.p = p


    @staticmethod
    def inverse_modulo(num, p):
        x0, x1, n = 1, 0, p
        while n != 0:
            q, num, n = num // n, n, num % n
            x0, x1 = x1, x0 - q * x1
        return x0 % p


    # Addition of two EC points
    def __add__(self, other):
        R = EllipticCurvePoint()
        R.a = self.a
        R.b = self.b
        R.p = self.p

        dx = (other.x - self.x) % self.p
        dy = (other.y - self.y) % self.p

        if self.x == other.x:
            if self.y == other.y:
                l = ((3 * self.x ** 2 + self.a) * \
                    EllipticCurvePoint.inverse_modulo(2 * self.y, self.p)) % self.p
            elif self.y == -other.y:
                return 0
            else:
                return float('inf')
        else:
            l = (dy * EllipticCurvePoint.inverse_modulo(dx, self.p)) % self.p

        R.x = (l * l - self.x - other.x) % self.p
        R.y = (l * (self.x - R.x) - self.y) % self.p
        return R


    def __sub__(self, other):
        other.y = -other.y
        R = self.__add__(other)
        return R


    # Multiplication of integer value and EC point
    def __rmul__(self, other):
        R = EllipticCurvePoint(self.x, self.y, self.a, self.b, self.p)
        temp = EllipticCurvePoint(self.x, self.y, self.a, self.b, self.p)
        n = other - 1
        while n != 0:
            if n % 2 != 0:
                R += temp
                n -= 1
            n //= 2
            temp = temp + temp
        return R
