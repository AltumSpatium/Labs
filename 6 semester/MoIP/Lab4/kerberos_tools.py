from Crypto.Cipher import AES
from Crypto import Random


def extend_password(password):
	extended_password = password
	if len(password) < 16:
		for i in range(16 - len(password)):
			extended_password += '0'
	return extended_password.encode()


def encrypt(text, password):
	iv = Random.new().read(16)
	obj = AES.new(extend_password(password), AES.MODE_CFB, iv)
	cipher = iv + obj.encrypt(text.encode())
	return cipher


def decrypt(cipher, password):
	iv = cipher[:16]
	obj = AES.new(extend_password(password), AES.MODE_CFB, iv)
	text = obj.decrypt(cipher)[16:]
	return text
