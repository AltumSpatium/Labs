from des import DES


class TripleDES(object):
    def __init__(self, key1, key2, key3=None):
        self.crypter1 = DES(key1)
        self.crypter2 = DES(key2)
        self.crypter3 = DES(key3) if key3 else None
        self.mode = 'EEE3' if key3 else 'EEE2'


    def encrypt(self, data):
        data = self.crypter1.encrypt(data)
        data = self.crypter2.encrypt(data)
        if self.mode == 'EEE2':
            return self.crypter1.encrypt(data)
        elif self.mode == 'EEE3':
            return self.crypter3.encrypt(data)


    def decrypt(self, data):
        if self.mode == 'EEE2':
            data = self.crypter1.decrypt(data)
        elif self.mode == 'EEE3':
            data = self.crypter3.decrypt(data)
        data = self.crypter2.decrypt(data)
        return self.crypter1.decrypt(data)
