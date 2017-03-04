#ifndef PACKET_H
#define PACKET_H

#include "../Server/server.h"
#include "../Session/Session.h"

#include<string>
#include<cstring>
#include<stdio.h>

//class Server;
class Session;

class Packet{
public:
	Packet();
	~Packet();

	void SetSession(Session* session);
	Session* GetSession();
	void SetType(int type);
	int GetType();

	int ReadInt(int& n);
	float ReadFloat(float& f);
	char* ReadString(char*);

	void PushInt(int&);
	void PushFloat(float&);
	void PushString(char*);

	void SetId(int id);
	void AddHead(int iType);
	void AddTail();
	int mId;
	unsigned char mStreamBuff[29948];
	int mContenLen;
	static int ReadInt(unsigned char* p){
		int i =0;
		i = (i)|p[0];
		i = (i<<8)|p[1];
		i = (i<<8)|p[2];
		i = (i<<8)|p[3];
		return i;
	}
private:
	//need a malloc module
	
	int mCurrPos;
	int mType;
	Session* mSession;
	const static int mMaxLen=2048;
	
	
};

#endif