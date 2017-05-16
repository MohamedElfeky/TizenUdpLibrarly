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

int my_listen_callback(struct sockaddr_in sender, const std::string& msg){
    return 0;
}
int main(void)
{
    stringstream ss("bla|bla");
    string s;

    while (getline(ss, s, '|')) {
        cout << s << endl;
    }
    // std::cout<<split("abc|def|geh",'|');
    // udp_simple_socket * socket =udp_simple_socket::getInstance();
    // socket -> bind_port(2322);
    // socket -> listen();
    // socket -> set_listen_callback_func(my_listen_callback);
}