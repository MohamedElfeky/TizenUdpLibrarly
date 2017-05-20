#include "udp_simple_socket.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <pthread.h>
#include <stdlib.h>
#include <vector>
using namespace std;
struct user {
    string token;
    struct sockaddr_in  global;
    struct sockaddr_in  local;
};

vector<struct user> user_vector;

int enroll_user(struct sockaddr_in&sender, stringstream & ss){
    string id,local_ip,local_port;
    bool temp = getline(ss,id,'|');
    if(temp){
        temp = getline(ss,local_ip,'|');
        if(temp){
            temp = getline(ss,local_port,'|');
            if(temp){
                if(udp_simple_socket::check_address(local_ip,local_port)){
                    cout<< "ip success"<<endl;
                    
                }else{
                    cout<< "ip fail"<<endl;
                }
            }
        }
    }
    
}

int my_listen_callback(struct sockaddr_in sender, const std::string& msg){
    stringstream ss(msg);
    string s;
    bool temp = getline(ss,s,'|');
    if(temp){
        if(s.compare("enroll")==0){
            cout << "enroll part" << endl;
            enroll_user(sender,ss);
        }
    }
    cout<<"this is callback"<<endl;
}
int main(void)
{
    udp_simple_socket * socket =udp_simple_socket::getInstance();
    socket -> bind_port(2322);
    socket -> listen();
    socket -> set_listen_callback_func(my_listen_callback);
    while(1){
        sleep(1);
    }
    return 0;
}