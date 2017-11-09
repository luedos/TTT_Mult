#pragma once
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#include <WinSock2.h>
//#include <iostream>
//#include <string>
#include "MultBase.h"
//#pragma comment (lib, "ws2_32.lib")

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