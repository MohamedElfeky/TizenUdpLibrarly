from socket import *
csock = socket(AF_INET, SOCK_DGRAM)
csock.sendto("Hello, World!".encode('utf-8'), ('127.0.0.1',2322))
s, addr = csock.recvfrom(1024)
print(s)
print(addr)
