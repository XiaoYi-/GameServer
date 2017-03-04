#ifndef SESSION_H
#define SESSION_H

#define MAX_PACKET_LEN 2048
#include "../GameServer/GameServer.h"
#include "../Packet/Packet.h"
//#include "../Thread/EventThread.h"
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <stdlib.h>

class GameServer;
class Packet;

class Session{
public:
	Session();
	~Session();

	bool Decode(int& iType,int& iStartPos,int& iLen);
	void OnStreamRecieve(struct bufferevent *b, void *arg);
	void SendPacket(Packet*);

	void Init(GameServer* server,struct bufferevent *,evutil_socket_t fd,struct sockaddr *sa,char* sIp,int iPort);
	//EventThread* mEventThread;
	struct bufferevent* mBuffev;
	GameServer* mGameServer;
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