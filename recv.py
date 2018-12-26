import socket
import binascii
import struct

HOST = ''
PORT = 8887

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
    sock.bind((HOST, PORT))
    sock.listen(1)
    print('Listen on %s:%s' % (HOST, PORT))
    while True:
        conn, addr = sock.accept()
        with conn:
            print('Connected by %s:%d' % addr)
            while True:
                s = conn.recv(4)
                if not s:
                    break
                size_buf = struct.unpack('!i', s)[0]
                print('Ready to accept a file with size %d bytes' % size_buf)
                s = conn.recv(size_buf)
                if not s:
                    break
                b = bytearray(s)
                h = binascii.hexlify(b)
                print(h)
