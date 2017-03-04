#ifndef GAMESERVER_H
#define GAMESERVER_H

#include "../Object/Player.h"
#include "../Object/ObjectHolder.h"
#include "../Packet/Packet.h"
#include "../Thread/DispatchThread.h"
#include "../Thread/EventThread.h"
#include "../EnginScript/ScriptObj.h"
#include "../EnginScript/EnginScript.h"

#include <pthread.h>
#include <Python.h>
#include <vector>
#include <map>

#define MAX_STR  1024

class Packet;
class EventThread;
class GameServer;
class Player;

//static EventThread* gEventThread = NULL;
static Packet* gPacket = NULL;
static Packet* gCurrSendPacket=NULL;

static std::map<int,Session*> gMapSession;

class GameServer:public ScriptObj{ //
public:
	GameServer();
	~GameServer();
	void OnRecievePacket(Packet* p);
	Packet* GetNextPacket();
	//bool InitNet();//int EventThread 
	bool StartServer();//
	EventThread* GetEventThread(){ return this->mEventThread; }
	//void Run();

	void PreparePacket(int);
	void PacketInt(int);
	void PacketDouble(double);
	void PacketStr(char*);
	int UnpackInt();
	double UnpacketDouble();
	char* UnpacketStr();
	void SendPacket();

	Player* CreatePlayer(int id);
	Object* FindObjById(double id);
	void ExePacket();
	void SetFunc(PyObject *p){this->pFuncObj=p;}


	/*
	virtual void PreparePacket(int);
	virtual void PacketInt(int);
	virtual void PacketDouble(double);
	virtual void PacketStr(char*);
	virtual int UnpackInt();
	virtual double UnpacketDouble();
	virtual char* UnpacketStr();
	virtual void SendPacket();
	virtual Object* FindObjById(double id);
	*/

	Packet* mCurrSendPacket;
	Packet* mCurrExePacket;
	static char StrBuff[MAX_STR];
	PyObject* pFuncObj;
	EventThread* mEventThread;
private:
	std::vector<Packet*> mPacket_front;
	std::vector<Packet*> mPacket_back;

	pthread_mutex_t mPacket_front_mutex_lock;
	pthread_mutex_t mPacket_back_mutex_lock;
};

static Session* FindSessionById(int rid){
	std::map<int ,Session* >::iterator item=gMapSession.find(rid);
   	if(item != gMapSession.end()){
   		return item->second;
   	}
   	return NULL;
}

#endif
