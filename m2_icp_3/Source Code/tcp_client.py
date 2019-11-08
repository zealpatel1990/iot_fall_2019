#!/usr/bin/env python

import socket,time

ip = '192.168.1.7'
port = 5005
BUFFER_SIZE = 1024
MESSAGE = "tweet 23"

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((ip, port))
time.sleep(5)
s.send(MESSAGE)
data = s.recv(BUFFER_SIZE)
s.close()

print "received data:", data
