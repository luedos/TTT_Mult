#pragma once
#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>
#include <string>
#pragma comment (lib, "ws2_32.lib")

using namespace std;

enum PacketType
{
	P_StartGame,
	P_Turn,
	P_Grid,
	P_Win
};

class MultBase
{
public:

	MultBase() {}

	SOCKET Connection;

	virtual bool ProcessPacket(PacketType packetType) { return false; }

	virtual bool ConnectTo(const char* IP, int PORT) { return false; }

	class GM_MP_Game* MyGame;
	
	SOCKADDR_IN addr;

	int addrlen = sizeof(addr);

	void CloseMyConnection()
	{
		closesocket(Connection);
	}
	
	bool SendBool(bool _Bool);

	bool GetBool(bool& _Bool);

	bool SendString(string _String);

	bool SendInt(int _Int);

	bool GetInt(int &_Int);

	bool SendPacket(PacketType _Packet);

	bool GetPacket(PacketType& _Packet);

	bool GetString(string& _String);

	bool IsProcessing = false;

	bool ConnectionClosed;

};