import socket
import time

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
host = '127.0.0.1'
port = 5000

sock.bind((host, port))

while True:
    count = 0
    msg = f'This is message {count}'
    sock.sendto(msg.encode(), ('127.0.0.1', 69))
    time.sleep(2)
