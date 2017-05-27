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

int connect_two_user(struct user&user_A,struct user&user_B){//두 유저한테 연결정보 갱신 알려줌
    stringstream to_stream_A,to_stream_B;
    string connect_string("c");
    to_stream_A <<  connect_string;
    to_stream_A <<'|'<<inet_ntoa(user_A.global.sin_addr);
    to_stream_A <<'|'<<ntohs(user_A.global.sin_port);
    to_stream_A <<'|'<<inet_ntoa(user_B.global.sin_addr);
    to_stream_A <<'|'<<ntohs(user_B.global.sin_port);
    to_stream_A <<'|'<<inet_ntoa(user_B.local.sin_addr);
    to_stream_A <<'|'<<ntohs(user_B.local.sin_port)<<'|';
    string to_user_A(to_stream_A.str());
    to_stream_B<<connect_string<<'|';
    to_stream_B<<inet_ntoa(user_B.global.sin_addr);
    to_stream_B<<'|'<<ntohs(user_B.global.sin_port);
    to_stream_B<<'|'<<inet_ntoa(user_A.global.sin_addr);
    to_stream_B<<'|'<<ntohs(user_A.global.sin_port);
    to_stream_B<<'|'<<inet_ntoa(user_A.local.sin_addr);
    to_stream_B<<'|'<<ntohs(user_A.local.sin_port)<<'|';
    string to_user_B(to_stream_B.str());
    cout<<to_user_A<<endl<<to_user_B<<endl;
    udp_simple_socket::getInstance()->sendSync(user_A.global,to_user_A);
    udp_simple_socket::getInstance()->sendSync(user_B.global,to_user_B);
}

int enroll_user(struct sockaddr_in& sender, stringstream & ss){
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
                        struct user to_connect;//연결할놈
                        int to_connect_count=0;//연결할놈 카운트
                        bool to_refresh = false;
                        for(vi = user_vector.begin();vi!=user_vector.end();){
                            bool id_cmp = vi->id.compare(id)==0;
                            bool token_cmp = vi->token.compare(token)==0;
                            if(id_cmp){
                                if(token_cmp){
                                    //TODO ip 비교하고 다르면 연결 refresh
                                    cout<<"같은놈인디 아이피 비교해봐야겠구먼"<<endl;
                                    if(sender.sin_addr.s_addr != vi->global.sin_addr.s_addr||sender.sin_port!=vi->global.sin_port){
                                        cout<<"아이피를 비교했더니 어이쿠 아이피나 포트가 달라졌네"<<endl;
                                        vi = user_vector.erase(vi); //현재 아이피 지우기
                                        to_refresh = true;
                                        continue;
                                    }else{
                                    }
                                }else{
                                    //TODO 연결 해제하고 새로운 토큰에 연결하기
                                    cout<<token<<"과 연결을 해제합니다"<<endl;
                                    vi = user_vector.erase(vi); //현재 아이피 지우기
                                    to_refresh = true;
                                    continue;
                                }
                            }else{
                                if(token_cmp){
                                    //TODO 현재 연결된 애가 2명이면 reject, 1명이면 연결해주기
                                    if(to_connect_count == 0){
                                        to_connect = *vi;
                                        to_connect_count ++;
                                        to_refresh = true;
                                    }else{
                                        //TODO 여기서 return 해주고 해당 토큰이 busy하다고 알려줘야함
                                        return 0;
                                    }
                                }
                            }
                            vi++;
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
                        if(to_refresh&&to_connect_count==1)connect_two_user(temp_user,to_connect);
                    }else{
                        cout<< "ip fail"<<endl;
                    }
                }
            }
        }
    }
    
}

int my_listen_callback(struct sockaddr_in sender, const std::string& msg){
    udp_simple_socket::getInstance()->sendSync(sender,"hi");
    stringstream ss(msg);
    string s;
    bool temp = getline(ss,s,'|');
    if(temp){
        if(s.compare("e")==0){
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