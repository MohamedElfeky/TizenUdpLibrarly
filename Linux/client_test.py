from socket import *
def test():
    csock1 = socket(AF_INET, SOCK_DGRAM).getsockopt
    csock2 = socket(AF_INET, SOCK_DGRAM)
    csock1.sendto("Hello, World!".encode('utf-8'), ('127.0.0.1',2322))
    csock2.sendto("Hello, World!".encode('utf-8'), ('127.0.0.1',2322))
    s, addr = csock1.recvfrom(1024)
    print(s)
    print(addr)
def send_to_addr():
    socket.gethostname()
    csock1 = socket(AF_INET, SOCK_DGRAM)
    csock1.bind(('localhost', 0))
    print(csock1.getsockname())
    s, addr = csock1.recvfrom(1024)
    print(s)
    print(addr)
