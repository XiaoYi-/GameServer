
/*event thread deal with io event*/

#ifndef EVENTTHREAD_H
#define EVENTTHREAD_H

#include "thread.h"
#include "../packet/packet.h"
#include <event2/event.h>

#include <queue>
#include <unistd.h>

class Session;

class EventThread:public Thread{// 
public:
	EventThread();
	//virtual ~EventThread();
	void Run();
	void Exit();

	void OnBuffRead(struct bufferevent *b, void *arg);
	void OnBuffWrite(struct bufferevent *b, void *arg);

	bool Init();
	event_base* mevbase;

private:
	std::queue<Packet*> mPacketBack;
	std::queue<Packet*> mPacketFront;
};

#endif