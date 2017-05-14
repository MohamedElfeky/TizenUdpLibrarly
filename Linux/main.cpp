#include "main.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

int main(){
    udp_simple_socket* socket = new udp_simple_socket("127.0.0.1",2322);
    std::cout<<socket->get_port()<<socket->get_addr()<<std::endl;
    socket->listen();
    sleep(10);
    // socket->recv();
    return 0;
}
udp_simple_socket::udp_simple_socket(const std::string& addr, int port)
    : f_port(port)
    , f_addr(addr)
{
    char decimal_port[16];
    snprintf(decimal_port, sizeof(decimal_port), "%d", f_port);
    decimal_port[sizeof(decimal_port) / sizeof(decimal_port[0]) - 1] = '\0';

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    int r(getaddrinfo(addr.c_str(), decimal_port, &hints, &f_addrinfo));
    if(r != 0 || f_addrinfo == NULL)
    {
        std::cout<<"addrinfo fail";
    }

    f_socket = socket(f_addrinfo->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);
    if(f_socket == -1)
    {
        freeaddrinfo(f_addrinfo);
        std::cout<<"addrinfo fail";
    }
    struct sockaddr_in my_addr;
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family      = AF_INET;
    my_addr.sin_port        = htons(port);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    if( bind(f_socket, (const struct sockaddr *)&my_addr,sizeof(my_addr)) == -1)
    {
        std::cout<<"bind fail";
    }
        
}
udp_simple_socket::~udp_simple_socket()
{
    freeaddrinfo(f_addrinfo);
    close(f_socket);
}

/** \brief Retrieve a copy of the socket identifier.
 *
 * This function return the socket identifier as returned by the socket()
 * function. This can be used to change some flags.
 *
 * \return The socket used by this UDP client.
 */
int udp_simple_socket::get_socket() const
{
    return f_socket;
}

/** \brief Retrieve the port used by this UDP client.
 *
 * This function returns the port used by this UDP client. The port is
 * defined as an integer, host side.
 *
 * \return The port as expected in a host integer.
 */
int udp_simple_socket::get_port() const
{
    return f_port;
}

/** \brief Retrieve a copy of the address.
 *
 * This function returns a copy of the address as it was specified in the
 * constructor. This does not return a canonalized version of the address.
 *
 * The address cannot be modified. If you need to send data on a different
 * address, create a new UDP client.
 *
 * \return A string with a copy of the constructor input address.
 */
std::string udp_simple_socket::get_addr() const
{
    return f_addr;
}

bool udp_simple_socket::listen() const  
{
    pthread_t p_thread;
    int thr_id;
    int a = 100;

    printf("Before Thread\n"); 
    thr_id = pthread_create(&p_thread, NULL, udp_simple_socket::startRecv, (void *)this);
    if (thr_id < 0)
    {
        perror("thread create error : ");
        exit(0);
    }

    // 식별번호 p_thread 를 가지는 쓰레드를 detach 
    // 시켜준다.
    pthread_detach(p_thread);
    // pause();
    return true;
}
bool udp_simple_socket::sendSync(std::string message) const  
{
    pthread_t p_thread;
    int thr_id;
    int status;
    int a = 100;
    struct two_arg_struct socket_and_message;
    socket_and_message.arg1 = (void *)this;
    socket_and_message.arg2 = (void *)&message;
    printf("Before Thread\n"); 
    thr_id = pthread_create(&p_thread, NULL, udp_simple_socket::startSend, (void *)&socket_and_message);
    if (thr_id < 0)
    {
        perror("thread create error : ");
        exit(0);
    }

    // 식별번호 p_thread 를 가지는 쓰레드를 detach 
    // 시켜준다.
    pthread_join(p_thread,(void **)&status);
    // pause();
    return true;
}
void *  udp_simple_socket::startRecv(void * This)
{
    ((udp_simple_socket *)This)->recv();
    return NULL;
}
void *  udp_simple_socket::startSend(void * args)
{
    struct udp_simple_socket::two_arg_struct * temp = (struct udp_simple_socket::two_arg_struct * )args;
    ((udp_simple_socket *)temp->arg1)->send((std::string *)temp->arg2);
    return NULL;
}

void  udp_simple_socket::recv()
{
    char buf[1024];
    int  recv_len;
    int s = f_socket;
    slen = sizeof(si_other); 
     while(1)
    {
        printf("Waiting for data...");
        fflush(stdout);
         
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, 1024, 0, (struct sockaddr *) &si_other, (socklen_t*)&slen)) == -1)
        {
            printf("recvfrom fail()");
        }
        
        //print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n" , buf);
        send(&temp_message);
    }
    return;
}
void udp_simple_socket::send(std::string * message)const
{
    
    if (sendto(f_socket, message->c_str(), message->length(), 0, (struct sockaddr*) &si_other, slen) == -1)
    {
        printf("sendto fail()");
    }
}