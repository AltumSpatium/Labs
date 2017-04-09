import socket
import random
import time
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
	cipher = iv + obj.encrypt(text)
	return cipher


def decrypt(cipher, password):
	iv = cipher[:16]
	obj = AES.new(extend_password(password), AES.MODE_CFB, iv)
	text = obj.decrypt(cipher)[16:]
	return text


def generate_session_key():
	random.seed(time.time())
	session_key = ""
	for i in range(16):
		session_key += chr(random.randint(97, 122))
	return session_key


def create_session_ticket(server_name, client_name, client_addr, exp_date, session_key, secret_key):
	client_info = "{0},{1},{2},{3}".format(client_name, client_addr, exp_date, session_key)
	client_info = encrypt(client_info.encode(), secret_key)
	session_ticket = bytes([len(server_name)]) + server_name.encode() + client_info
	return session_ticket


def decode_session_ticket(session_ticket, secret_key):
	server_name_len = session_ticket[0]
	server_name = ""
	for i in range(1, server_name_len + 1):
		server_name += chr(session_ticket[i])
	client_info = decrypt(session_ticket[server_name_len + 1:], secret_key)
	return (server_name, client_info.decode().split(','))


def authentication(password, as_addr, client_name, tgs_name):
	socket_as = socket.socket()
	socket_as.connect(as_addr)

	message = client_name + ',' + tgs_name
	print('Sending message to AS: "' + message + '"')
	socket_as.send(message.encode('utf-8'))

	session_key = decrypt(socket_as.recv(1024), password).decode()
	session_ticket = socket_as.recv(1024)
	print('Received from AS: session key -', session_key,', session ticket -', decode_session_ticket(session_ticket, 'tgssecretkey'))

	socket_as.close()

	return (session_key, session_ticket)
