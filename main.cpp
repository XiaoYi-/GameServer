
#include "./Thread/DispatchThread.h"
#include "./Packet/Packet.h"

int main(int argc,char** argv){

	GameServer* server = new GameServer();
	server->StartServer();

	return 0;
}