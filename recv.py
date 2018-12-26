import socket
import binascii
import struct
import numpy as np
import matplotlib.pyplot as plt 

HOST = ''
PORT = 8887

buf = bytearray(18000)
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
    sock.bind((HOST, PORT))
    sock.listen(1)
    print('Listen on %s:%s' % (HOST, PORT))
    while True:
        conn, addr = sock.accept()
        with conn:
            print('Connected by %s:%d' % addr)
            img = np.array([], dtype=np.uint8)
            while True:
                #s = conn.recv(4)
                #if not s:
                #    break
                #size_buf = struct.unpack('!i', s)[0]
                #print('Ready to accept a file with size %d bytes' % size_buf)
                s = conn.recv(1024)
                if not s:
                    break
                b = bytearray(s)
                d = np.frombuffer(b, dtype=np.uint8)
                img = np.concatenate((img, d))
                print(d)
                #h = binascii.hexlify(b)
                #print(h)
            plt.show(img)
