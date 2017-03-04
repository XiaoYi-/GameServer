#include "DispatchThread.h"

EventThread* DispatchThread::mEventThread = new EventThread();
std::map<int,Session*> DispatchThread::mSessionMap = std::map<int,Session*>();

DispatchThread::DispatchThread(GameServer* server,event_base* base,int iPort){
	this->mPort = iPort;
	this->mevbase = base;
	this->mGameServer = server;
}

DispatchThread::~DispatchThread(){
	event_base_free(this->mevbase);
}

void DispatchThread::Init(){

}

void DispatchThread::InsertSession(int iPort,Session* se){
	DispatchThread::mSessionMap.insert(std::make_pair(iPort,se));
}

Session* DispatchThread::GetSession(char* sIp,int iPort){
	std::map<int,Session*>::iterator item = DispatchThread::mSessionMap.find(iPort);
	if(item == DispatchThread::mSessionMap.end())
		return NULL;
	return item->second;
}

static void buff_readcb(struct bufferevent *b, void *arg){
	printf("%s\n", "----------readcb-----------------");
	Session* session = (Session*)(arg);
	EventThread* evThread = session->mGameServer->GetEventThread();
	evThread->OnBuffRead(b,session);
}

static void buff_writecb(struct bufferevent *b, void *arg){
	EventThread* evThread = (EventThread*)arg;
	evThread->OnBuffWrite(b,arg);
}


void DispatchThread::Connection_notify(const EventThread* evThread,struct evconnlistener *listener, evutil_socket_t fd,struct sockaddr *sa){
	// create buffevent and session
	// call back is eventthread me
	printf("%s\n","------create connect-----");
	GameServer* server = this->mGameServer;
	struct bufferevent * buffev;
	struct event_base *base =  server->GetEventThread()->mevbase;

	sockaddr_in* client_addr = (sockaddr_in*)(sa);
	char* sIp = (char*)malloc(16);
	sprintf(sIp,"%s",inet_ntoa(client_addr->sin_addr));
	//memcpy(sIp,client_addr->sin_addr,15);
	int iPort = client_addr->sin_port;

	printf("%s  %d\n", sIp,iPort); 

	Session* session = new Session();
	buffev = bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
	if(not buffev){
		printf("%s\n", "create bufferevent error!");
		return;
	}
	bufferevent_setcb(buffev, buff_readcb, buff_writecb, NULL,session);
	bufferevent_enable(buffev, EV_WRITE);
	bufferevent_enable(buffev, EV_READ);
	
	session->Init(server,buffev,fd,sa,sIp,iPort);
	DispatchThread::InsertSession(iPort,session);
}

void DispatchThread::Run(){
	while(true){
		printf("%s %d\n", "bbb",this->mPort);
		event_base_dispatch(this->mevbase);
		printf("%s %d\n", "aaa",this->mPort);
		//break;
	}
	//pthread_exit(0);
}

void DispatchThread::Exit(){

}