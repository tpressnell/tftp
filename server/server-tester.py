import socket
import time

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
host = '127.0.0.1'
port = 10001

sock.bind((host, port))

count = 0
while True:
    msg = f'This is message {count}'
    print(msg)
    sock.sendto(msg.encode(), ('127.0.0.1', 10000))
    print('after sendto')
    count = count + 1
    time.sleep(2)
