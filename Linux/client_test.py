from socket import *
import os

def test():
    csock1 = socket(AF_INET, SOCK_DGRAM)
    csock1.sendto("Hello, World!".encode('utf-8'), ('192.168.0.109',23272))
    s, addr = csock1.recvfrom(1024)
    print(s)
    print(addr)
    
def recv_from_addr():
    socket.gethostname()
    csock1 = socket(AF_INET, SOCK_DGRAM)
    csock1.bind(('localhost', 0))
    print(csock1.getsockname())
    s, addr = csock1.recvfrom(1024)
    print(s)
    print(addr)
    
def send_message(my_str):
    csock1 = socket(AF_INET,SOCK_DGRAM)
    csock1.sendto(my_str.encode('utf-8'), ('127.0.0.1',23272))
    

def get_lan_ip():
    ip = gethostbyname(gethostname())
    return str(ip)

def enroll(token,id):
    temp_str = "e|"+token+'|'+id+'|'+get_lan_ip()+'|'
    csock1 = socket(AF_INET,SOCK_DGRAM)
    csock1.bind(('', 0))
    temp_str += str(csock1.getsockname()[1])+'|'
    csock1.sendto(temp_str.encode('utf-8'), ('165.194.17.3',23272))
    print(temp_str)
    
def enroll_test():
    enroll("A",'1')
    enroll("A",'2')
    enroll("B",'3')
    enroll("B",'3')
    enroll("C",'4')
    enroll("C",'5')
    enroll("C",'6')
    
def enrollandlisten(token,id):
    temp_str = "enroll|"+token+'|'+id+'|'+get_lan_ip()+'|'
    csock1 = socket(AF_INET,SOCK_DGRAM)
    csock1.bind(('', 0))
    temp_str += str(csock1.getsockname()[1])+'|'
    csock1.sendto(temp_str.encode('utf-8'), ('165.194.17.3',23272))
    print(temp_str)
    s, addr = csock1.recvfrom(1024)
    print(s)
    print(addr)
