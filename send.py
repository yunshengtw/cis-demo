import socket

HOST = '127.0.0.1'
PORT = 8887

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
buf = bytearray(56)
buf[0] = 10
buf[1] = 20
s.sendall(buf)
