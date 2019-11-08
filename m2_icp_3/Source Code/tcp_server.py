import socket
import sys
from time import sleep as delay
ip = ''
port = 5005
BUFFER_SIZE = 20;s = socket.socket(socket.AF_INET, socket.SOCK_STREAM);s.bind((ip, port));s.listen(3)
conn, addr = s.accept()
print 'Connection address:', addr
data = conn.recv(BUFFER_SIZE)
print "received data:", data
delay(2)
conn.close()
