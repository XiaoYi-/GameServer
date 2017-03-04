#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>

#include "Packet/Packet.h"
#include "clientObj/connectObj.h"

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include <pthread.h>

#define MYPORT  9992
#define BUFFER_SIZE 1024

static ConnectObj* conObj = new ConnectObj();

static void buff_readcb(struct bufferevent *b, void *arg){
	printf("%s\n", "buff_readcb");
	conObj->OnReadPacket();
}

static void buff_writecb(struct bufferevent *b, void *arg){

}

static void* proc(void* t){
	ConnectObj* obj = (ConnectObj*)t;
	while(true){
		event_base_dispatch(obj->base);
	}
}

int main()
{
    ///定义sockfd
    int sock_cli = socket(AF_INET,SOCK_STREAM, 0);

    ///定义sockaddr_in
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MYPORT);  ///服务器端口
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  ///服务器ip

    ///连接服务器，成功返回0，错误返回-1
    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
       // perror("connect");
        exit(1);
    }

    struct event_base *base =  event_base_new();
    struct bufferevent* buffev = bufferevent_socket_new(base,sock_cli,BEV_OPT_CLOSE_ON_FREE);
	if(not buffev){
		printf("%s\n", "create bufferevent error!");
		return -1;
	}
	bufferevent_setcb(buffev, buff_readcb, buff_writecb, NULL,conObj);
	bufferevent_enable(buffev, EV_WRITE);
	bufferevent_enable(buffev, EV_READ);

	conObj->Init(buffev);
	conObj->base = base;

	pthread_t pid;
	int ret=pthread_create(&pid,NULL,proc,(void*)conObj);

    char sendbuf[BUFFER_SIZE];
    char recvbuf[BUFFER_SIZE];
    for(int j=1;j<=1;++j)
    {
		Packet* p = new Packet();
		p->AddHead(j);
		char str[] = "client_send_to_server";
		p->PushString(str);
		p->AddTail();
	    memcpy(sendbuf,p->mStreamBuff,p->mContenLen);
		printf("mContenLen=%d\n", p->mContenLen);
		send(sock_cli, sendbuf,p->mContenLen,0); ///发送
		delete p;
    }

    while(true){
    	conObj->ExePacket();
    }

    return 0;
}