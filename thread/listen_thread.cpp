#include "listen_thread.h"

EventThread* listenThread::mEventThread = new EventThread();
std::map<int,Session*> listenThread::mSessionMap = std::map<int,Session*>();

listenThread::listenThread(Server* server):mServer(server){
	mevbase = event_base_new();
}

listenThread::~listenThread(){
	event_base_free(this->mevbase);
}

bool listenThread::start_listener(int iPort){
	struct sockaddr_in sin;
	//struct event_base* evbase = this->mevbase;
	struct evconnlistener* evlistener;
	//struct event timeout ;
	//struct timeval tv;

	memset(&sin,0,sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(iPort);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	//evbase = event_base_new();

	evlistener = evconnlistener_new_bind(mevbase,connection_notify,this,LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,
		-1,(struct sockaddr*)&sin,sizeof(sin));

	if(!evlistener){
		//printf("%s\n", "listener error");
		return false;
	}
	return true;
}

bool listenThread::Init(){
	pthread_t pid;
	int ret=pthread_create(&pid,NULL,Proc,(void*)this);
	return true;
}

void listenThread::InsertSession(int iPort,Session* se){
	listenThread::mSessionMap.insert(std::make_pair(iPort,se));
}

Session* listenThread::GetSession(char* sIp,int iPort){
	std::map<int,Session*>::iterator item = listenThread::mSessionMap.find(iPort);
	if(item == listenThread::mSessionMap.end())
		return NULL;
	return item->second;
}

static void buff_readcb(struct bufferevent *b, void *arg){
	//printf("%s\n", "----------readcb-----------------");
	Session* session = (Session*)(arg);
	EventThread* evThread = session->mServer->GetEventThread();
	evThread->OnBuffRead(b,session);
}

static void buff_writecb(struct bufferevent *b, void *arg){
	EventThread* evThread = (EventThread*)arg;
	evThread->OnBuffWrite(b,arg);
}


void listenThread::Connection_notify(const EventThread* evThread,struct evconnlistener *listener, evutil_socket_t fd,struct sockaddr *sa){
	Server* server = this->mServer;
	struct bufferevent * buffev;
	struct event_base *base =  server->GetEventThread()->mevbase;

	sockaddr_in* client_addr = (sockaddr_in*)(sa);
	char* sIp = (char*)malloc(16);
	sprintf(sIp,"%s",inet_ntoa(client_addr->sin_addr));
	int iPort = client_addr->sin_port;

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
	listenThread::InsertSession(iPort,session);
}

void listenThread::Run(){
	while(true){
		event_base_dispatch(this->mevbase);
	}
}

void listenThread::Exit(){

}