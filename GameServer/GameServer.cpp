#include "GameServer.h"

char GameServer::StrBuff[MAX_STR];

GameServer::GameServer(){}

GameServer::~GameServer(){}

static void ProcPro(void* att){ // deal with pro
	GameServer* server = (GameServer*)att;
	server->ExePacket();
}

static void ProCallBack(void* att){

}

static void ProTimeOut(void* att){

}

static void ProSave(void* att){

}

static void* MainLoopProc(void* att){ // main loop
	//GameServer* server = (GameServer*)att;

	while(true){
		ProcPro(att);
		ProCallBack(att);
		ProTimeOut(att);
		ProSave(att);
	}
}

/*改成多个工作线程从任务池中拿出消息处理，不知道python状态机那边支不支持。*/
void GameServer::ExePacket(){
	//printf("%s\n", "1 GameServer::ExePacket");
	{
		MutexLock mutexlock(_frontMutexLock);

		this->mCurrExePacket = NULL;
		if(this->mPacket_front.empty()){

			MutexLock mutexlock(_backMutexLock);

			if(not this->mPacket_back.empty()){
				std::vector<Packet*>::iterator iter = this->mPacket_back.begin();
				for(iter;iter!=this->mPacket_back.end();)
				{
					printf("%s\n", "mPacket_front.push_back");
					this->mPacket_front.push_back(*iter);
					iter = this->mPacket_back.erase(iter);
				}
			}
		}
		std::vector<Packet*>::iterator item = this->mPacket_front.begin();
		if(item != this->mPacket_front.end()){
			this->mPacket_front.erase(item);
			this->mCurrExePacket = *item;
		}
		//pthread_mutex_unlock(&this->mPacket_front_mutex_lock);
	}

	//printf("%s\n", "3 GameServer::ExePacket");
	if(this->mCurrExePacket == NULL){
		return;
	}
	if(not this->pFuncObj){
		printf("%s\n", "not this->pFuncObj");
		return;
	}
	printf("%s\n", "2 GameServer::ExePacket");
	//int resultInt=0;
	int iType = this->mCurrExePacket->GetType();
	PyObject* pArgc = Py_BuildValue("(i)",iType);
	PyObject* result = PyEval_CallObject(this->pFuncObj, pArgc); //call script fun
}

void GameServer::OnRecievePacket(Packet* p){
	//pthread_mutex_lock(&this->mPacket_back_mutex_lock);
	MutexLock mutexlock(_backMutexLock);
	this->mPacket_back.push_back(p);
	//printf("%s\n", "GameServer::OnRecievePacket");
	//pthread_mutex_unlock(&this->mPacket_back_mutex_lock);
}

Packet* GameServer::GetNextPacket(){
	return NULL;
}

/*bool GameServer::InitNet(){
	
}*/
bool GameServer::StartServer(){
	// init decode memery mamager

	//InitNet(this);

	//pthread_mutex_init(&this->mPacket_front_mutex_lock,NULL);
	//pthread_mutex_init(&this->mPacket_back_mutex_lock,NULL);

	this->mEventThread = new EventThread();
	//pthread_t pid;
	//int ret=pthread_create(&pid,NULL,Proc,this->mEventThread );
	if(not this->mEventThread->Init()){
		printf("%s\n", "EventThread init error!");
		return false;
	}
	
	StartListener(this,9990);//db server
	StartListener(this,9991);//client
	StartListener(this,9992);//gate erver

	InitScriptServer(this); // init script
	MainLoopProc(this);
	
	return true;
}
//	Packet* mCurrSendPacket;
//	Packet* mCurrExePacket;

void GameServer::PreparePacket(int iType){
	printf("%s %d\n", "GameServer::PreparePacket",iType);
	if(this->mCurrSendPacket){
		//delete this->mCurrSendPacket;
		this->mCurrSendPacket = NULL;
	}
	this->mCurrSendPacket = new Packet();
	this->mCurrSendPacket->AddHead(iType);
	Session* sesseion = DispatchThread::GetFirstSession();
	if(not sesseion){
		printf("%s\n", "if(not sesseion)");
		return;
	}
	this->mCurrSendPacket->SetSession(sesseion);
}
void GameServer::PacketInt(int i){
	this->mCurrSendPacket->PushInt(i);
}
void GameServer::PacketDouble(double d){
	float f = (float)d;
	this->mCurrSendPacket->PushFloat(f);
}
void GameServer::PacketStr(char* str){
	printf("%s\n", "GameServer::PacketStr");
	this->mCurrSendPacket->PushString(str);
}
int GameServer::UnpackInt(){
	int i = 0;
	this->mCurrExePacket->ReadInt(i);
	printf("%s %d\n", "GameServer::UnpackInt",i);
	return i;
}
double GameServer::UnpacketDouble(){
	float f = 0;
	this->mCurrExePacket->ReadFloat(f);
	return f;
}
char* GameServer::UnpacketStr(){
	this->StrBuff;
	this->mCurrExePacket->ReadString((char*)this->StrBuff);
	printf("GameServer::UnpacketStr %s\n", this->StrBuff);
	return (char*)this->StrBuff;
}
void GameServer::SendPacket(){
	printf("%s\n", "GameServer::SendPacket");
	this->mCurrSendPacket->AddTail();
	Session* session = this->mCurrSendPacket->GetSession();
	session->SendPacket(this->mCurrSendPacket);
	//this->mCurrSendPacket = NULL;
}
Object* GameServer::FindObjById(double id){
	return ObjectHolder::FindObjById((int)id);
}

Player* GameServer::CreatePlayer(int id){
	Player* p = new Player();
	p->SetId(id);
	ObjectHolder::InsertObj(p,id);
}