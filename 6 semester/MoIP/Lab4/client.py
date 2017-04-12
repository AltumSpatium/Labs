import socket
import kerberos_tools as kt

HOST = 'localhost'

SERVER_ADDR = (HOST, 9090)
AS_ADDR = (HOST, 9091)
TGS_ADDR = (HOST, 9092)

CLIENT_NAME = 'alex'
TGS_NAME = 'localtgs'
SERVER_NAME = 'localserver'

server_key = None
server_ticket = None	

session_key, session_ticket = kt.authentication('password', AS_ADDR, CLIENT_NAME, TGS_NAME)
server_key, server_ticket = kt.accessing(TGS_ADDR, CLIENT_NAME, SERVER_NAME, session_key, session_ticket)
