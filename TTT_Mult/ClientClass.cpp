#include "stdafx.h"
#include "GameInclude/GM_MP_Game.h"
#include "Game.h"
#include "GameInclude/ClientClass.h"

ClientClass::ClientClass(GM_MP_Game* GameRef)
{
	MyGame = GameRef;

	clientptr = this;

	ConnectionClosed = true;
}

bool ClientClass::ProcessPacket(PacketType packetType)
{
	cout << "Client : Taken packet : ";
	switch (packetType)
	{
	case P_StartGame:
	{
		cout << "P_StartGame" << endl;
		int W, H, D, MyIndex;
		bool MyTurn;
		
		if (!GetInt(W))
			return false;
		if (!GetInt(H))
			return false;
		if (!GetInt(D))
			return false;
		if (!GetBool(MyTurn))
			return false;
		if (!GetInt(MyIndex))
			return false;

		MyGame->ChangeGrid(W, H, D);

		MyGame->MyIndex = MyIndex;

		MyGame->IsMyTurn = MyTurn;

		MyGame->IsOponentReady = true;

		if(MyGame->IsMeReady)
			MyGame->NewRound();

		break;
	}
	case P_Turn:
	{
		cout << "P_Turn";
		int Ind;
		if (!GetInt(Ind))
			return false;

		MyGame->MakeTurn(Ind, true, !MyGame->IsMyTurn);

		MyGame->IsMyTurn = !MyGame->IsMyTurn;

		cout << " : " << Ind << endl;
		
		break;
	}
	case P_Grid:
	{
		cout << "P_Grid" << endl;
		break;
	}
	case P_Win:
	{
		cout << "P_Score" << endl;
		int X,O;
		bool IsXWin;
		if (!GetInt(X))
			return false;
		if (!GetInt(O))
			return false;
		if (!GetBool(IsXWin))
			return false;

		MyGame->XWins = X;
		MyGame->OWins = O;
		MyGame->WinMenu(IsXWin);

		break;
	}
	default:
	{
		cout << "Client : Unrecognize packet type" << endl;
		break;
	}
	}

	return true;
}

bool ClientClass::ConnectTo(const char * IP, int PORT)
{
	ConnectionClosed = false;

	WSADATA wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Client : Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	addr.sin_addr.s_addr = inet_addr(IP); // Can be error!
	addr.sin_port = htons(PORT);
	addr.sin_family = AF_INET; // AF_INET6 - IPv4 socket 

	int enable = 1;
	setsockopt(Connection, SOL_SOCKET, SO_REUSEADDR, (const char*)&enable, sizeof(int));

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0)
	{
		MessageBoxA(NULL, "Client : Failed to connect", "Error", MB_OK | MB_ICONERROR);
		ConnectionClosed = true;
		return false;
	}
	cout << "Connected to Server" << endl;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MyThread, NULL, NULL, NULL);

	return true;
}

void ClientClass::MyThread()
{
	PacketType packetType;
	while (true)
	{

		if (!clientptr->GetPacket(packetType))
			break;

		
		clientptr->IsProcessing = true;

		if (!clientptr->ProcessPacket(packetType))
			break;

		clientptr->IsProcessing = false;
	}

	cout << "Client : Lost connection to server" << endl;
	clientptr->CloseMyConnection();

	clientptr->IsProcessing = false;

	clientptr->ConnectionClosed = true;
	//clientptr->MyGame->MyGame->LoadMainMenu();
}
