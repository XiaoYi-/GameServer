#ifndef GAMESERVER_H
#define GAMESERVER_H

#include "../object/player.h"
#include "../object/object_holder.h"
#include "../packet/packet.h"
#include "../thread/listen_thread.h"
#include "../thread/event_thread.h"
#include "../script/script_oj.h"
#include "../script/engin_script.h"
#include "../thread/mutex.h"

#include <pthread.h>
#include <Python.h>
#include <vector>
#include <map>

#define MAX_STR  1024

class Packet;
class EventThread;
class Server;
class Player;

//static EventThread* gEventThread = NULL;
static Packet* gPacket = NULL;
static Packet* gCurrSendPacket=NULL;

static std::map<int,Session*> gMapSession;

class Server:public ScriptObj{ //
public:
	Server();
	~Server();
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


	Packet* mCurrSendPacket;
	Packet* mCurrExePacket;
	static char StrBuff[MAX_STR];
	PyObject* pFuncObj;
	EventThread* mEventThread;
private:
	std::vector<Packet*> mPacket_front;
	std::vector<Packet*> mPacket_back;

	//pthread_mutex_t mPacket_front_mutex_lock;
	//pthread_mutex_t mPacket_back_mutex_lock;
	Mutex _frontMutexLock;
	Mutex _backMutexLock;
};

static Session* FindSessionById(int rid){
	std::map<int ,Session* >::iterator item=gMapSession.find(rid);
   	if(item != gMapSession.end()){
   		return item->second;
   	}
   	return NULL;
}

#endif
