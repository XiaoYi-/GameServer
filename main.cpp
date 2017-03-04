
#include "./Thread/DispatchThread.h"
#include "./Packet/Packet.h"

//static GameServer* gGameServer = NULL;
//static Packet* gPacket = NULL;

int main(int argc,char** argv){
	//int iPort = 9998;
	//StartListener(iPort);
	//ProcLoop()

/*	std::vector<Packet*> packet_list;
	Packet* p1 = new Packet();
	p1->SetId(1);
	packet_list.push_back(p1);

	Packet* p2 = new Packet();
	p2->SetId(2);
	packet_list.push_back(p2);

	std::vector<Packet*>::iterator v = packet_list.begin();
	printf("%d\n",(*v)->mId);*/

	GameServer* server = new GameServer();
	//server->InitNet();
	server->StartServer();

	return 0;
}