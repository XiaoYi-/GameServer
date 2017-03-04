#include "EventThread.h"
#include "../Session/Session.h"


static int g_packet_send_write_fd = 0;
static int g_packet_recive_write_fd = 0;

static void packet_recive_cb(evutil_socket_t s, short what, void *ptr)
{
	EventThread* evThread = (EventThread*)(ptr);
	printf("%s\n", "packet_recive_cb");
	return;
}

static void packet_send_cb(evutil_socket_t s, short what, void *ptr)
{
	EventThread* evThread = (EventThread*)(ptr);
	printf("%s\n", "packet_send_cb");
	return;
}


EventThread::EventThread(){
	//this->mevbase = event_base_new();
}
//EventThread::~EventThread(){}

void EventThread::Run(){
	while(true){
		event_base_dispatch(this->mevbase);
	}
}

void EventThread::Exit(){}

bool EventThread::Init(){
	event_base* base = event_base_new();
	if(base == NULL){
		printf("%s\n", "EventThread event_base_new error!");
		return false;
	}
	this->mevbase = base;
	//if(evthread_use_pthreads()!=0){
	//	printf("%s\n", "evthread_use_pthreads error!");
	//}
	event* ev;
	int filedes[2];
	if(pipe(filedes) !=0){
		printf("%s\n", "1 pipe error!");
		return false;
	}
	//filedes[0] // read
	//filedes[1] // write
	g_packet_recive_write_fd = filedes[1];
	ev = event_new(base, filedes[0],EV_READ,packet_recive_cb,(void*)this);
	if(ev == NULL){
		printf("%s\n", "1 event_new error!");
		return false;
	}
	event_add(ev,NULL);

	if(pipe(filedes) !=0){
		printf("%s\n", "2 pipe error!");
		return false;
	}

	g_packet_send_write_fd = filedes[1];
	ev = event_new(base,filedes[0],EV_READ,packet_send_cb,(void*)this);
	if(ev == NULL){
		printf("%s\n", "2 event_new error!");
		return false;
	}
	event_add(ev,NULL);

	pthread_t pid;
	int ret=pthread_create(&pid,NULL,Proc,(void*)this);
	return true;
}

void EventThread::OnBuffRead(struct bufferevent *b, void *arg){
	printf("%s\n", "EventThread::OnBuffRead");
	Session* session = (Session*)arg;
	session->OnStreamRecieve(session->mBuffev,NULL);
}
void EventThread::OnBuffWrite(struct bufferevent *b, void *arg){
	printf("%s\n", "EventThread::OnBuffWrite");

}