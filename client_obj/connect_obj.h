#ifndef CONNECTOBJ_H
#define CONNECTOBJ_H

#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include "../packet/Packet.h"

#include <vector>

class ConnectObj{
public:
	ConnectObj(){}
	~ConnectObj(){}
	void Init(struct bufferevent* bev);
	void OnReadPacket();
	bool Decode(int& iType,int& iStartPos,int& iLen);

	void ExePacket();
	struct bufferevent* mBuffev;
	struct event_base *base ;
private:
	int mCurrPos;
	int mMaxLen;
	unsigned char* mStreamBuff;
	
	std::vector<Packet*> mPacketBuff;
};

#endif