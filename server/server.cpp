#include "server.h"

char Server::StrBuff[MAX_STR];

Server::Server(){}

Server::~Server(){}

static void ProcPro(void* att){ // deal with pro
	Server* server = (Server*)att;
	server->ExePacket();
}

static void ProCallBack(void* att){

}

static void ProTimeOut(void* att){

}

static void ProSave(void* att){

}

static void* MainLoopProc(void* att){ // main loop
	//Server* server = (Server*)att;

	while(true){
		ProcPro(att);
		ProCallBack(att);
		ProTimeOut(att);
		ProSave(att);
	}
}

/*改成多个工作线程从任务池中拿出消息处理，不知道python状态机那边支不支持。*/
void Server::ExePacket(){
	//printf("%s\n", "1 Server::ExePacket");
	{
		MutexLock mutexlock(_frontMutexLock);

		Packet* iter = NULL;
		this->mCurrExePacket = NULL;
		if(this->mPacket_front.empty()){

			MutexLock mutexlock(_backMutexLock);

			if(not this->mPacket_back.empty()){
				while(not this->mPacket_back.empty())
				{
					printf("%s\n", "mPacket_front.push_back");
					this->mPacket_front.push(iter);
					this->mPacket_back.pop();
				}
			}
		}
		
		if(not this->mPacket_front.empty()){
			iter = this->mPacket_front.front();
			this->mPacket_front.pop();
			this->mCurrExePacket = iter;
		}
		//pthread_mutex_unlock(&this->mPacket_front_mutex_lock);
	}

	//printf("%s\n", "3 Server::ExePacket");
	if(this->mCurrExePacket == NULL){
		return;
	}
	if(not this->pFuncObj){
		printf("%s\n", "not this->pFuncObj");
		return;
	}
	//printf("%s\n", "2 Server::ExePacket");
	//int resultInt=0;
	int iType = this->mCurrExePacket->GetType();
	PyObject* pArgc = Py_BuildValue("(i)",iType);
	PyObject* result = PyEval_CallObject(this->pFuncObj, pArgc); //call script fun
}

void Server::OnRecievePacket(Packet* p){
	//pthread_mutex_lock(&this->mPacket_back_mutex_lock);
	MutexLock mutexlock(_backMutexLock);
	this->mPacket_back.push(p);
	//printf("%s\n", "Server::OnRecievePacket");
	//pthread_mutex_unlock(&this->mPacket_back_mutex_lock);
}

Packet* Server::GetNextPacket(){
	return NULL;
}

/*bool Server::InitNet(){
	
}*/
bool Server::StartServer(){
	// init decode memery mamager

	//InitNet(this);

	//pthread_mutex_init(&this->mPacket_front_mutex_lock,NULL);
	//pthread_mutex_init(&this->mPacket_back_mutex_lock,NULL);

	this->mEventThread = new EventThread();
	this->mListenThread = new listenThread(this);
	//pthread_t pid;
	//int ret=pthread_create(&pid,NULL,Proc,this->mEventThread );
	
	this->mListenThread->start_listener(9990);//db server
	this->mListenThread->start_listener(9991);//client
	this->mListenThread->start_listener(9992);//gate erver

	if(not this->mEventThread->Init() or not this->mListenThread->Init()){
		return false;
	}

	InitScriptServer(this); // init script
	MainLoopProc(this);
	
	return true;
}
//	Packet* mCurrSendPacket;
//	Packet* mCurrExePacket;

void Server::PreparePacket(int iType){
	printf("%s %d\n", "Server::PreparePacket",iType);
	if(this->mCurrSendPacket){
		//delete this->mCurrSendPacket;
		this->mCurrSendPacket = NULL;
	}
	this->mCurrSendPacket = new Packet();
	this->mCurrSendPacket->AddHead(iType);
	Session* sesseion = listenThread::GetFirstSession();
	if(not sesseion){
		printf("%s\n", "if(not sesseion)");
		return;
	}
	this->mCurrSendPacket->SetSession(sesseion);
}
void Server::PacketInt(int i){
	this->mCurrSendPacket->PushInt(i);
}
void Server::PacketDouble(double d){
	float f = (float)d;
	this->mCurrSendPacket->PushFloat(f);
}
void Server::PacketStr(char* str){
	printf("%s\n", "Server::PacketStr");
	this->mCurrSendPacket->PushString(str);
}
int Server::UnpackInt(){
	int i = 0;
	this->mCurrExePacket->ReadInt(i);
	printf("%s %d\n", "Server::UnpackInt",i);
	return i;
}
double Server::UnpacketDouble(){
	float f = 0;
	this->mCurrExePacket->ReadFloat(f);
	return f;
}
char* Server::UnpacketStr(){
	this->StrBuff;
	this->mCurrExePacket->ReadString((char*)this->StrBuff);
	printf("Server::UnpacketStr %s\n", this->StrBuff);
	return (char*)this->StrBuff;
}
void Server::SendPacket(){
	printf("%s\n", "Server::SendPacket");
	this->mCurrSendPacket->AddTail();
	Session* session = this->mCurrSendPacket->GetSession();
	session->SendPacket(this->mCurrSendPacket);
	//this->mCurrSendPacket = NULL;
}
Object* Server::FindObjById(double id){
	return ObjectHolder::FindObjById((int)id);
}

Player* Server::CreatePlayer(int id){
	Player* p = new Player();
	p->SetId(id);
	ObjectHolder::InsertObj(p,id);
}