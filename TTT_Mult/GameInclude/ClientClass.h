#pragma once
//#define  _WINSOCK_DEPRECATED_NO_WARNINGS
//#include <WinSock2.h>
//#include <iostream>
//#include <string>
#include "MultBase.h"
//#pragma comment (lib, "ws2_32.lib")

using namespace std;

class ClientClass : public MultBase
{
public:

	ClientClass(class GM_MP_Game* GameRef);

	bool ProcessPacket(PacketType packetType) override;

	bool ConnectTo(const char* IP, int PORT) override;

	
private:

	static void MyThread();

	
};
 
static ClientClass* clientptr;