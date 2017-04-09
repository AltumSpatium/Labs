import socket
import kerberos_tools as kt

HOST = 'localhost'

SERVER_ADDR = (HOST, 9090)
AS_ADDR = (HOST, 9091)
TGS_ADDR = (HOST, 9092)

CLIENT_NAME = 'alex'
TGS_NAME = 'localtgs'

server_key = None
server_ticket = None


def authentication(password, as_addr, client_name, tgs_name):
	socket_as = socket.socket()
	socket_as.connect(as_addr)

	message = client_name + ',' + tgs_name
	print('Sending message to AS: "' + message + '"')
	socket_as.send(message.encode('utf-8'))

	session_key = kt.decrypt(socket_as.recv(1024), password).decode()
	session_ticket = socket_as.recv(1024)
	print('Received from AS: session key -', session_key,', session ticket -', session_ticket)

	socket_as.close()

	return (session_key, session_ticket)
	

authentication('password', AS_ADDR, CLIENT_NAME, TGS_NAME)
