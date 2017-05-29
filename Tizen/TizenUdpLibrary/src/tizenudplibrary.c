/**
 * This file contains the exported symbol.
 */
#include "tizenudplibrary.h"
#include <net_connection.h>
// This is an example of an exported method.

int tul_socket;
struct sockaddr_in si_server,si_other,si_my_global,si_other_global,si_other_local;
int i, slen,f_port =23165;
char recv_buf[BUFF_SIZE] = "";
char send_buf[BUFF_SIZE] = "";
char str1[BUFF_SIZE]="",str2[BUFF_SIZE]="";
char * my_ip = NULL;
int(* my_listen_call_back)(int ,int);
int is_connected = NOT_CONNECTED, is_init = 0;
Ecore_Thread * listen_thread;

int tul_init(char * server_ip,int port)
{
	connection_h header;
	if ((tul_socket=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1){//소켓 생성
		dlog_print(DLOG_DEBUG, LOG_TAG, "socket() failed.\n");
		return -1;
	}
	tul_bind_port(f_port);
	memset((char *) &si_server, 0, sizeof(si_server));// 서버 ip 초기화
	si_server.sin_family = AF_INET;
	si_server.sin_port = htons(port);
	if (inet_aton(server_ip, &si_server.sin_addr)==0) {
		dlog_print(DLOG_DEBUG, LOG_TAG, "inet_aton() failed.\n");
		return -1;
	}
	is_init = 1;
	if(connection_create(&header)!=0){
		dlog_print(DLOG_DEBUG,LOG_TAG,"connection header fail");
	}
	if(connection_get_ip_address(header,CONNECTION_ADDRESS_FAMILY_IPV4,&my_ip)!=0){
		dlog_print(DLOG_DEBUG,LOG_TAG,"ip_address not obtained");
	}
	dlog_print(DLOG_DEBUG,LOG_TAG,"my ip is %s, size is %d",my_ip,strlen(my_ip));
	return 0;
}

int tul_connect(char * token,char * id)
{
	if(is_init==0){
		dlog_print(DLOG_DEBUG,LOG_TAG,"tul_init is needed");
		return -1;
	}
	sprintf(send_buf,"e|%s|%s|%s|%d|",token,id,my_ip,f_port);
	return send_packet(si_server,send_buf,strlen(send_buf));
}

int tul_enroll(char * token,char * id)
{
	if(is_init==0){
		dlog_print(DLOG_DEBUG,LOG_TAG,"tul_init is needed");
		return -1;
	}
	sprintf(send_buf,"e|%s|%s|%s|%d|",token,id,my_ip,f_port);
	return send_packet(si_server,send_buf,strlen(send_buf));
}

int tul_send_sync(char * message,int message_len){

	if(is_connected==NOT_CONNECTED){
		dlog_print(DLOG_DEBUG,LOG_TAG,"not connected");
		return -1;
	}
	return 0;
}

int tul_send_async(char * message,int message_len,int(* callBack)(int ,int))
{

	if(is_connected==NOT_CONNECTED){
		dlog_print(DLOG_DEBUG,LOG_TAG,"not connected");
		return -1;
	}
	return 0;
}

int tul_bind_port(int port){// 소켓에 port를 bind 한다.
	if(is_init==0){
		dlog_print(DLOG_DEBUG,LOG_TAG,"tul_init is needed");
		return -1;
	}
    f_port = port;

	struct sockaddr_in my_addr;
	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family      = AF_INET;
	my_addr.sin_port        = htons(port);
	my_addr.sin_addr.s_addr = INADDR_ANY;

	if( bind(tul_socket, (const struct sockaddr *)&my_addr,sizeof(my_addr)) == -1)
	{
		dlog_print(DLOG_DEBUG,LOG_TAG,"tul_bind() fail");
		return -1;
	}
	return 0;
}


void tul_listen(void *data, Ecore_Thread *thread){
	int  recv_len,i=0;
	char * ip_str=NULL,*port_str=NULL;
	char	*ptr;
	int	 ndx;
	slen = sizeof(si_other);
	while(1)
	{
		dlog_print(DLOG_DEBUG,LOG_TAG,"Waiting for data...");
		memset(recv_buf, 0, BUFF_SIZE);
		//try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(tul_socket, recv_buf,BUFF_SIZE, 0, (struct sockaddr *) &si_other,(socklen_t*)&slen)) == -1)
		{
			dlog_print(DLOG_DEBUG,LOG_TAG,"recvfrom() fail");
		}

		//print details of the client/peer and the data received

		dlog_print(DLOG_DEBUG,LOG_TAG,"Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
		dlog_print(DLOG_DEBUG,LOG_TAG,"Data: %s\n" , recv_buf);
		if(recv_buf[0]=='c'){
			//TODO 서로 연결을 해야 하느니라아아

			dlog_print(DLOG_DEBUG,LOG_TAG,"connect part" );

			dlog_print(DLOG_DEBUG,LOG_TAG,"before str is %s\n" ,recv_buf);


			ptr = strtok( recv_buf, "|");

			ip_str = strtok( NULL, "|");
			port_str = strtok( NULL, "|");

			memset((char *) &si_my_global, 0, sizeof(si_my_global));// 서버 ip 초기화
			si_my_global.sin_family = AF_INET;
			si_my_global.sin_port = htons(atoi(port_str));
			if (inet_aton(ip_str, &si_my_global.sin_addr)==0) {
				dlog_print(DLOG_DEBUG, LOG_TAG, "inet_aton() failed.\n");
				return;
			}
			dlog_print(DLOG_DEBUG,LOG_TAG,"my global %s:%d\n", inet_ntoa(si_my_global.sin_addr), ntohs(si_my_global.sin_port));

			ip_str = strtok( NULL, "|");
			port_str = strtok( NULL, "|");
			memset((char *) &si_other_global, 0, sizeof(si_other_global));// 서버 ip 초기화
			si_other_global.sin_family = AF_INET;
			si_other_global.sin_port = htons(atoi(port_str));
			if (inet_aton(ip_str, &si_other_global.sin_addr)==0) {
				dlog_print(DLOG_DEBUG, LOG_TAG, "inet_aton() failed.\n");
				return;
			}
			dlog_print(DLOG_DEBUG,LOG_TAG,"other global %s:%d\n", inet_ntoa(si_other_global.sin_addr), ntohs(si_other_global.sin_port));

			ip_str = strtok( NULL, "|");
			port_str = strtok( NULL, "|");
			memset((char *) &si_other_local, 0, sizeof(si_other_local));// 서버 ip 초기화
			si_other_local.sin_family = AF_INET;
			si_other_local.sin_port = htons(atoi(port_str));
			if (inet_aton(ip_str, &si_other_local.sin_addr)==0) {
				dlog_print(DLOG_DEBUG, LOG_TAG, "inet_aton() failed.\n");
				return;
			}
			dlog_print(DLOG_DEBUG,LOG_TAG,"other local %s:%d\n", inet_ntoa(si_other_local.sin_addr), ntohs(si_other_local.sin_port));
			ecore_thread_run(tul_connect_other,NULL,NULL,NULL);

		}
//		if(m_listen_callback ==NULL){
//			udp_simple_socket::sendSync(temp_message);
//		}else{
//			m_listen_callback(si_other,temp_message);
//		}
	}
}

void tul_listen_end(void *data, Ecore_Thread *thread){
	dlog_print(DLOG_DEBUG,LOG_TAG,"tul_listen() ended");
}

void tul_listen_cancel(void *data, Ecore_Thread *thread){
	dlog_print(DLOG_DEBUG,LOG_TAG,"tul_listen() canceled");
}

int tul_start_listen(void * data){
	listen_thread =  ecore_thread_feedback_run(tul_listen, NULL,tul_listen_end, NULL,data, EINA_FALSE);
	return 0;
}

int tul_stop_listen(){
	ecore_thread_cancel(listen_thread);
	return 0;
}

int tul_add_listen_callback(int(* callBack)(int ,int)){
	my_listen_call_back = callBack;
	return 0;
}

int send_packet(const struct sockaddr_in to_send,char * message,int message_len){
	int i = sendto(tul_socket, message,message_len, 0, (struct sockaddr*)&to_send, sizeof(to_send));
	dlog_print(DLOG_DEBUG,LOG_TAG,"sendto() is %d\n",i);
	if(i==-1){
		dlog_print(DLOG_DEBUG,LOG_TAG,"sendto() failed");
		return -1;
	}
	return 0;
}
void tul_connect_other(void *data, Ecore_Thread *thread){
	int i;
	for(i=0;i<5;i++){//TODO 안받으면  l|f, 받으면 l|t
		send_packet(si_other_local,"l|",2);
		usleep(10000);
	}
	for(i=0;i<5;i++){//TODO 안받으면  g|f, 받으면 g|t
		send_packet(si_other_global,"g|",2);
		usleep(10000);
	}
	for(i=0;i<5;i++){//TODO 릴레이로 안받으면 r|f 받으면 r|t
		send_packet(si_server,"r|id|r|f",2);
		usleep(10000);
	}
}
