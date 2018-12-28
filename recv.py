import socket
import binascii
import struct
import numpy as np
import matplotlib.pyplot as plt 
import time
from matplotlib import animation

HOST = ''
PORT = 8887
MSGLEN = 16641

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind((HOST, PORT))
sock.listen(1)
print('Listen on %s:%s' % (HOST, PORT))

#plt.ion()
#plt.show()
fig, ax = plt.subplots()
init = 255 - np.zeros((129, 129), dtype=np.uint8)
print(init)
x = plt.imread('cis_0.ppm')
display = ax.imshow(x, cmap='gray')

def animate(i):
    conn, addr = sock.accept()
    print('Connected by %s:%d' % addr)
    with conn:
        byte_recv = 0
        img = np.array([], dtype=np.uint8)
        while byte_recv < MSGLEN:
            #s = conn.recv(4)
            #if not s:
            #    break
            #size_buf = struct.unpack('!i', s)[0]
            #print('Ready to accept a file with size %d bytes' % size_buf)
            chunk = conn.recv(min(MSGLEN - byte_recv, 1024))
            if not chunk:
                break
            byte_recv = byte_recv + len(chunk)
            chunk = bytearray(chunk)
            chunk = np.frombuffer(chunk, dtype=np.uint8)
            img = np.concatenate((img, chunk))
            #h = binascii.hexlify(b)
            #print(h)
        print('Recv one image')
        img = np.flip(np.reshape(img, (129, 129)), axis=0)
        display.set_data(img)
        return (display,)
        #plt.imsave('x.pdf', img)
        #plt.imshow(img, cmap='gray')
        #plt.pause(1)

anim = animation.FuncAnimation(fig, animate, interval=50)
plt.show()
