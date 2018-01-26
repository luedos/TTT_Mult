#include "stdafx.h"
#include "GameInclude/GM_MP_Game.h"
#include "Game.h"
#include "GameInclude/ServerClass.h"

ServerClass::ServerClass(GM_MP_Game* GameRef)
{
	MyGame = GameRef;
	ConnectionClosed = true;
	serverptr = this;
}

bool ServerClass::ListenToConnection(const char* IP, int PORT, bool isPublic)
{
	ConnectionClosed = false;
	cout << "Connection In proccess" << endl;
	WSADATA wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Server : Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		return false;
	}
	


	if (isPublic)
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		addr.sin_addr.s_addr = inet_addr(IP);

	addr.sin_port = htons(PORT);
	addr.sin_family = AF_INET; // AF_INET6 - IPv4 socket 

	//SOCKET MyListen;

	sListen = socket(AF_INET, SOCK_STREAM, NULL);

	int enable = 1;
	setsockopt(sListen, SOL_SOCKET, SO_REUSEADDR, (const char*)&enable, sizeof(int));

	
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);


	//SOCKET LocalCon
		Connection = accept(sListen, (SOCKADDR*)&addr, &addrlen);
	

	if (Connection == SOCKET_ERROR)
	{
		std::cout << "Failed to accept new connection" << std::endl;

		ConnectionClosed = true;

		CloseMyConnection();

		return false;
	}
	


	std::cout << "Client Connected" << std::endl;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL);

	

	return true;
}



bool ServerClass::ProcessPacket(PacketType packetType)
{
	switch (packetType)
	{
	case P_StartGame:
	{
		MyGame->IsOponentReady = true;

		if (MyGame->IsMeReady)
			MyGame->NewRound();

		break;
	}
	case P_Turn:
	{
		cout << "Packet : P_Turn" << endl;
		int Ind;
		if (!GetInt(Ind))
			return false;


		bool TestBool = MyGame->MakeTurn(Ind, false, true);


		if (TestBool)
		{
			if (!SendPacket(P_Turn))
				return false;

			if (!SendInt(Ind))
				return false;

			if (MyGame->CanWinCheck(Ind, !(MyGame->MyIndex - 1)))
				MyGame->WinMenu(MyGame->MyIndex - 1);

			MyGame->IsMyTurn = true;
		}
			

		break;
	}
	case P_Grid:
	{
		cout << "Server : Incorrect packet Type : P_Grid" << endl;
		break;
	}
	case P_Win:
	{
		cout << "Server : Incorrect packet Type : P_Score" << endl;

		break;
	}
	default:
	{
		cout << "Server : Unrecognize packet type" << endl;
		break;
	}
	}

	return true;
}

void ServerClass::ClientThread()
{
	PacketType Packettype;

	while (true)
	{
		

		if (!serverptr->GetPacket(Packettype))
			break;

		serverptr->IsProcessing = true;
		if (!serverptr->ProcessPacket(Packettype))
			break;

		serverptr->IsProcessing = false;
	}

	cout << "Server : Lost connection to client" << endl;

	serverptr->CloseMyConnection();

	
	
	serverptr->ConnectionClosed = true;
	//serverptr->MyGame->MyGame->LoadMainMenu();

	serverptr->IsProcessing = false;
}
