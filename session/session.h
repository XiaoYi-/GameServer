#ifndef SESSION_H
#define SESSION_H

#define MAX_PACKET_LEN 2048
#include "../server/server.h"
#include "../packet/packet.h"
//#include "../thread/event_thread.h"
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <stdlib.h>

class Server;
class Packet;

class Session{
public:
	Session();
	~Session();

	bool Decode(int& iType,int& iStartPos,int& iLen);
	void OnStreamRecieve(struct bufferevent *b, void *arg);
	void SendPacket(Packet*);

	void Init(Server* server,struct bufferevent *,evutil_socket_t fd,struct sockaddr *sa,char* sIp,int iPort);
	//EventThread* mEventThread;
	struct bufferevent* mBuffev;
	Server* mServer;
private:
//Decode* mDecode;
int mCurrPos;
int mMaxLen;

unsigned char* mStreamBuff; //stream buff read from buffevent and buff there

evutil_socket_t mFd;
struct sockaddr* mSa;
char* mIp;
int mPort;
};

#endif