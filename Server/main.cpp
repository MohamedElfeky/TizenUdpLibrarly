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
    string id;
    struct sockaddr_in  global;
    struct sockaddr_in  local;
};

vector<struct user> user_vector;

int enroll_user(struct sockaddr_in&sender, stringstream & ss){
    string token,id,local_ip,local_port;
    bool temp = getline(ss,token,'|');
    if(temp){
        temp = getline(ss,id,'|');
        if(temp){
            temp = getline(ss,local_ip,'|');
            if(temp){
                temp = getline(ss,local_port,'|');
                if(temp){
                    if(udp_simple_socket::check_address(local_ip,local_port)){
                        cout<< "ip success"<<endl;

                        // 해당 아이디가 리스트에 있는지 검사
                        vector<struct user>::iterator vi;
                        struct user to_connect;
                        for(vi = user_vector.begin();vi!=user_vector.end();vi++){
                            bool id_cmp = vi->id.compare(id)==0;
                            bool token_cmp = vi->token.compare(token)==0;
                            if(id_cmp){
                                if(token_cmp){
                                    //TODO ip 비교하고 다르면 연결 refresh
                                    cout<<"같은놈인디 아이피 비교해봐야겠구먼"<<endl;
                                }else{
                                    //TODO 연결 해제하고 새로운 토큰에 연결하기
                                    cout<<token<<"과 연결을 해제합니다"<<endl;
                                }
                            }else{
                                if(token_cmp){
                                    //TODO 현재 연결된 애가 2명이면 reject, 1명이면 연결해주기
                                    cout<<"천천히합시다..."<<endl;
                                }
                            }
                        }

                        struct user temp_user = user();
                        struct sockaddr_in temp_addr =  sockaddr_in();

                        temp_addr.sin_family = AF_INET;
                        temp_addr.sin_addr.s_addr = inet_addr(local_ip.c_str());
                        temp_addr.sin_port = htons(atoi(local_port.c_str()));

                        temp_user.token = token;
                        temp_user.id = id;
                        temp_user.global = sender;
                        temp_user.local = temp_addr;
                        user_vector.push_back(temp_user);

                    }else{
                        cout<< "ip fail"<<endl;
                    }
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
    socket -> bind_port(23272);
    socket -> listen();
    socket -> set_listen_callback_func(my_listen_callback);
    while(1){
        sleep(1);
    }
    return 0;
}