#pragma once
#include "MultBase.h"


using namespace std;


class ServerClass : public MultBase
{
public:
	ServerClass(class GM_MP_Game* GameRef);

	bool ListenToConnection(const char* IP, int PORT, bool isPublic = false);

	SOCKADDR_IN addr;

private:

	bool ProcessPacket(PacketType packetType) override;

	static void ClientThread();

	int addrlen = sizeof(addr);
	SOCKET sListen;


};

static ServerClass* serverptr;