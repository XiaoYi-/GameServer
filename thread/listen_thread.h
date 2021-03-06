/*
  This exmple program provides a trivial server program that listens for TCP
  connections on port 9995.  When they arrive, it writes a short message to
  each client connection, and closes each connection once it is flushed.

  Where possible, it exits cleanly in response to a SIGINT (ctrl-c).
*/

#ifndef DISPATCHTHREAD_H
#define DISPATCHTHREAD_H

#include "../session/session.h"
#include "../server/server.h"
//#include "event_thread.h"
#include "thread.h"

#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#ifndef WIN32
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <sys/socket.h>
#endif

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>



#include <sys/types.h>
#include <event2/event-config.h>
#include <sys/stat.h>
#include <time.h>
#ifdef _EVENT_HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#include <event2/event_struct.h>

static void connection_notify(struct evconnlistener *listener, evutil_socket_t fd,
struct sockaddr *sa, int socklen, void *user_data);
///////////////listenThread

/*deal with conn event*/

class Server;
class EventThread;

class listenThread:public Thread
{
public:
	listenThread(Server* server);
	~listenThread();
	bool Init();
	bool start_listener(int iPort);
	void Connection_notify(const EventThread* evThread,struct evconnlistener *listener, evutil_socket_t fd,struct sockaddr *sa);
	virtual void Run();
	virtual void Exit();
	static EventThread* mEventThread;

	static void InsertSession(int iPort,Session* se);
	static Session* GetSession(char* sIp,int iPort);
	static Session* GetFirstSession(){
		std::map<int,Session*>::iterator item = listenThread::mSessionMap.begin();
		if(item == listenThread::mSessionMap.end())
			return NULL;
		return item->second;
	}
private:
	Server* mServer;
	event_base* mevbase;
	//std::vector<int> mPortList;

	static std::map<int,Session*> mSessionMap;
};


static void
timeout_cb(evutil_socket_t fd, short event, void *arg)
{
    struct timeval newtime, difference,lasttime;
    struct event *timeout = (struct event *)arg;
    double elapsed;

    evutil_gettimeofday(&newtime, NULL);
    evutil_timersub(&newtime, &lasttime, &difference);
    elapsed = difference.tv_sec + (difference.tv_usec / 1.0e6);

    printf("timeout_cb called at %d: %.3f seconds elapsed.\n",(int)newtime.tv_sec, elapsed);
    lasttime = newtime;

    if (true){
        struct timeval tv;
        evutil_timerclear(&tv);
        tv.tv_sec = 2;
        event_add(timeout, &tv);
    }
}

static bool InitNet(){//Server* server
}

/*static bool start_listener(Server* server,int iPort){
	printf("-----start_listener-----\n");
	struct sockaddr_in sin;
	struct event_base* evbase;
	struct evconnlistener* evlistener;
	struct event timeout ;
	struct timeval tv;

	memset(&sin,0,sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(iPort);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	evbase = event_base_new();

	if(!evbase){
		printf("%s\n", "event_base_new error");
		return false;
	}

	event_assign(&timeout, evbase, -1, EV_PERSIST, timeout_cb, (void*)&timeout);
	evutil_timerclear(&tv);
	tv.tv_sec = 2;
	event_add(&timeout, &tv);

	listenThread* dispath = new listenThread(server,evbase,iPort);

	if(!dispath){
		printf("%s\n", "dispath new error");
		return false;
	}

	evlistener = evconnlistener_new_bind(evbase,connection_notify,dispath,LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,
		-1,(struct sockaddr*)&sin,sizeof(sin));

	if(!evlistener){
		printf("%s\n", "listener error");
		return false;
	}
	pthread_t pid;
	int ret=pthread_create(&pid,NULL,Proc,(void*)dispath);
	return true;
}*/

static void connection_notify(struct evconnlistener *listener, evutil_socket_t fd,struct sockaddr *sa, int socklen, void *user_data){
	listenThread* dispath = (listenThread*)(user_data);
	dispath->Connection_notify(NULL,listener,fd,sa);
}
#endif