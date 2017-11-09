#include "stdafx.h"
#include "GameInclude/GM_MP_Game.h"
#include "Game.h"

GM_MP_Game::GM_MP_Game(Game* InGame, Graphics* InGraph)
{
	MyGame = InGame;
	MyGraph = InGraph;

	IsMyTurn = true;
	MyIndex = 1;

	IsServer = true;

	ChangeGrid(5, 5, 3);
}


GM_MP_Game::~GM_MP_Game()
{
}

void GM_MP_Game::GM_EventTick(float DeltaTime)
{
	if (MyMultiplayer != nullptr)
		if (MyMultiplayer->ConnectionClosed)
			MyGame->LoadMainMenu();

}

void GM_MP_Game::GM_Start()
{

	IsMeReady = true;
	IsOponentReady = true;
	XWins = 0;
	OWins = 0;
	
	MyGraph->ClearEverything();

	if (IsServer)
		MyMultiplayer = &MyServer;
	else
		MyMultiplayer = &MyClient;


	Coordinates TestCoord;
	TestCoord.bRelativeW = true;
	TestCoord.bRelativeX = true;
	TestCoord.bRelativeY = true;
	
	TestCoord.X = 0.3;
	TestCoord.Y = 0.3;
	TestCoord.W = 0.8;
	TestCoord.H = 60;

	MyGraph->AddStaticText("Loading...", { 225,225,225,225 }, &TestCoord);

	MyGraph->RenderEverything(1);

	if (IsServer)
		if (MyServer.ListenToConnection(IP.c_str(), PORT, IP.empty()))
			NewRound();
		else
			MyGame->LoadMainMenu();
	else
		if (!MyClient.ConnectTo(IP.c_str(), PORT))
			MyGame->LoadMainMenu();
}

void GM_MP_Game::LoadData(string* IPRef, int PORTRef)
{
	IP = *IPRef;
	PORT = PORTRef;
}

void GM_MP_Game::NewRound()
{
	MyGraph->ClearEverything();

	XO_Buttons.clear();

	if (IsServer)
	{
		if (!MyServer.SendPacket(P_StartGame))
			cout << "Bad send : Packet" << endl;

		if (!MyServer.SendInt(XNumber))
			cout << "Bad send : W" << endl;

		if (!MyServer.SendInt(YNumber))
			cout << "Bad send : H" << endl;

		if (!MyServer.SendInt(Diagonal))
			cout << "Bad send : D" << endl;

		if (!MyServer.SendBool(!IsMyTurn))
			cout << "Bad send : Turn" << endl;

		int ClientIndex;
		if (MyIndex == 2)
			ClientIndex = 1;
		else
			ClientIndex = 2;

		if (!MyServer.SendInt(!(MyIndex - 1) + 1))
			cout << "Bad send : Index" << endl;
	}


	Coordinates TestCoord;
	TestCoord.bRelativeX = true;
	TestCoord.bRelativeY = true;
	TestCoord.bRelativeW = true;
	TestCoord.bRelativeH = true;
	
	
	TestCoord.X = 0.2;
	TestCoord.Y = 0.01;
	
	SomeSomeString = "X - " + to_string(XWins) + "  :  O - " + to_string(OWins);


	MyGraph->AddStaticText(SomeSomeString.c_str(), { 225,225,225,225 }, &TestCoord);

	float XDiag, YDiag;

	if (YNumber > XNumber)
	{
		YDiag = 0.6 / YNumber;
		XDiag = YDiag * 4.f / 3.f;
	}
	else
	{
		XDiag = 0.8 / XNumber;
		YDiag = XDiag * 0.75f;
	}

	TestCoord.W = XDiag * XNumber;
	TestCoord.H = YDiag * YNumber;


	TestCoord.X = 0.5 - TestCoord.W / 2;
	TestCoord.Y = 0.4 - TestCoord.H / 2;

	MyGraph->AddStaticImage("../TTT_Mult/Textures/TTT_Grid.png", &TestCoord, &GridRect);

	Coordinates NewCoord = TestCoord;
	NewCoord.W = XDiag;
	NewCoord.H = YDiag;


	for (int y = 0; y < YNumber; y++)
		for (int x = 0; x < XNumber; x++)
		{
			NewCoord.X = x * XDiag + TestCoord.X;
			NewCoord.Y = y * YDiag + TestCoord.Y;
			XO_Buttons.push_back(MyGraph->AddTTTButton(&NewCoord));
		}


	TestCoord.bRelativeH = false;
	TestCoord.H = 60;
	TestCoord.X = 0.1;
	TestCoord.Y = 0.7;
	TestCoord.W = 0.8;

	MyGraph->AddButton(MyGame, &Game::LoadMainMenu, "Exit Game", &TestCoord);



}

void GM_MP_Game::LoadForNewRound()
{
	IsMeReady = true;

	MyGraph->ClearEverything();

	XO_Buttons.clear();

	if (!IsOponentReady)
	{
		Coordinates TestCoord;
		TestCoord.bRelativeX = true;
		TestCoord.bRelativeY = true;
		TestCoord.bRelativeW = true;
		TestCoord.bRelativeH = true;

		const char* WaitingChar;
		if (IsServer)
			WaitingChar = "Waiting for the client";
		else
			WaitingChar = "Waiting for the server";

		TestCoord.X = 0.05;
		TestCoord.Y = 0.3;

		MyGraph->AddStaticText(WaitingChar, { 225,225,225,225 }, &TestCoord);

		TestCoord.bRelativeH = false;
		TestCoord.H = 60;
		TestCoord.W = 0.8;
		TestCoord.X = 0.1;
		TestCoord.Y = 0.5;

		MyGraph->AddButton(MyGame, &Game::LoadMainMenu, "Exit", &TestCoord);

		if (IsServer)
		{
			if (!MyServer.SendPacket(P_StartGame))
				cout << "Bad send : Packet" << endl;

			if (!MyServer.SendInt(XNumber))
				cout << "Bad send : W" << endl;

			if (!MyServer.SendInt(YNumber))
				cout << "Bad send : H" << endl;

			if (!MyServer.SendInt(Diagonal))
				cout << "Bad send : D" << endl;

			if (!MyServer.SendBool(!IsMyTurn))
				cout << "Bad send : Turn" << endl;

			int ClientIndex;
			if (MyIndex == 2)
				ClientIndex = 1;
			else
				ClientIndex = 2;

			if (!MyServer.SendInt(ClientIndex))
				cout << "Bad send : Index" << endl;
		}
		else
			MyClient.SendPacket(P_StartGame);
	}
	else
		if (IsServer)
			NewRound();		
		else
			MyClient.SendPacket(P_StartGame);

}

void GM_MP_Game::GM_End()
{
	if(MyMultiplayer != nullptr)
		MyMultiplayer->CloseMyConnection();
	MyMultiplayer = nullptr;
	MyGraph->ClearEverything();
	XO_Buttons.clear();
}

void GM_MP_Game::GM_Event(SDL_Event * EventRef)
{
	if (EventRef->type == SDL_MOUSEBUTTONUP)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		cout << "Event" << endl;
		
		if(IsMyTurn && IsMeReady)
		for(int i = 0; i < XO_Buttons.size(); i++)
			if (XO_Buttons.at(i)->MyRect.x < x &&
				XO_Buttons.at(i)->MyRect.y < y &&
				XO_Buttons.at(i)->MyRect.x + XO_Buttons.at(i)->MyRect.w > x &&
				XO_Buttons.at(i)->MyRect.y + XO_Buttons.at(i)->MyRect.h > y)
			{

				if (IsServer)
				{
					if (MakeTurn(i))
					{
						cout << "Server Made turn" << endl;

						if (!MyServer.SendPacket(P_Turn))
							cout << "Bad send : Packet" << endl;

						if (!MyServer.SendInt(i))
							cout << "Bad send : Turn index" << endl;

						if (CanWinCheck(i,MyIndex - 1))
							WinMenu(!(MyIndex-1));

						IsMyTurn = false;
					}
				}
				else
				{
					cout << "Client made turn. " << endl;

					if (!MyClient.SendPacket(P_Turn))
						cout << "Bad send : Packet" << endl;

					if (!MyClient.SendInt(i))
						cout << "Bad send : Int Turn" << endl;

				}


			}
	}
}

bool GM_MP_Game::MakeTurn(int InIndex, bool Hard, bool IsOponent)
{
	bool TestBoolLocal = true;

	int IndexToUse;
	if (IsOponent)
	{
		if (MyIndex == 1)
			IndexToUse = 2;
		else
			IndexToUse = 1;
	}
	else
		IndexToUse = MyIndex;

	if (!Hard)
	{
		bool TestBoolLocal = false;

		switch (MyPlaceRule)
		{
		case EPlaceRule::Regular:
		{
			TestBoolLocal = CanPlaceRegular(InIndex);
			break;
		}
		case EPlaceRule::Horse:
		{
			TestBoolLocal = CanPlaceHorse(InIndex, IsOponent);
			break;
		}
		default:
		{
			break;
		}
		}

		if (!TestBoolLocal)
			return false;
	}

	if (TestBoolLocal)
	{		
		XO_Buttons.at(InIndex)->TTTIndex = IndexToUse;
	}

	return true;
}



void GM_MP_Game::WinMenu(bool IsX)
{
	IsMeReady = false;
	IsOponentReady = false;

	Coordinates TestCoord;
	TestCoord.bRelativeW = true;
	TestCoord.bRelativeX = true;
	TestCoord.bRelativeY = true;

	TestCoord.X = 0.2;
	TestCoord.W = 0.6;
	TestCoord.Y = 0.3;
	TestCoord.H = 60;

	const char* CharToRender;

	if (IsX)
	{
		CharToRender = "X Wins";
		if (IsServer)
			++XWins;
	}
	else
	{
		CharToRender = "O Wins";
		if (IsServer)
			++OWins;
	}
	
	MyGraph->AddStaticText(CharToRender, { 0,225,0,225 }, &TestCoord, 2);

	TestCoord.Y = 0.5;

	MyGraph->AddButton(this, &GM_MP_Game::LoadForNewRound, "New Round", &TestCoord, { 70,70,70,180 }, { 30,30,30,200 }, { 50,50,50,200 }, 2);

	if (IsServer)
	{
		if (!MyServer.SendPacket(P_Win))
			cout << "Bad send : Win Packet" << endl;

		if (!MyServer.SendInt(XWins))
			cout << "Bad send : XWins" << endl;

		if (!MyServer.SendInt(OWins))
			cout << "Bad send : OWins" << endl;

		if (!MyServer.SendBool(IsX))
			cout << "Bad send : IsX";
	}

}

bool GM_MP_Game::CanWinCheck(int Num, bool IsO)
{
	
	int TestX, TestY;

	// XO_Buttons.at(Num)->TTTIndex

	// Проверка по +Х
	GetGridFromNum(Num, TestX, TestY);

	while (true)
	{
		if (TestX < XNumber)
			if (IsO + 1 == XO_Buttons.at(GetNumFromGrid(TestX, TestY))->TTTIndex)
				TestX++;
			else
				break;
		else
			break;
	}

	TestX--;

	int DiagNum = 0;
	while (true)
	{
		if (TestX >= 0)
			if (IsO + 1 == XO_Buttons.at(GetNumFromGrid(TestX, TestY))->TTTIndex)
			{
				DiagNum++;
				TestX--;
			}
			else
				break;
		else
			break;
	}

	if (DiagNum >= Diagonal)
		return true;

	// Проверка по +Х +Y

	GetGridFromNum(Num, TestX, TestY);

	while (true)
	{
		if (TestX < XNumber && TestY < YNumber)
			if (IsO + 1 == XO_Buttons.at(GetNumFromGrid(TestX, TestY))->TTTIndex)
			{
				TestX++;
				TestY++;
			}
			else
				break;
		else
			break;
	}

	TestX--;
	TestY--;

	DiagNum = 0;
	while (true)
	{
		if (TestX >= 0 && TestY >= 0)
			if (IsO + 1 == XO_Buttons.at(GetNumFromGrid(TestX, TestY))->TTTIndex)
			{
				DiagNum++;
				TestX--;
				TestY--;
			}
			else
				break;
		else
			break;
	}

	if (DiagNum >= Diagonal)
		return true;

	// Проверка по +Y
	GetGridFromNum(Num, TestX, TestY);

	while (true)
	{
		if (TestY < YNumber)
			if (IsO + 1 == XO_Buttons.at(GetNumFromGrid(TestX, TestY))->TTTIndex)
				TestY++;
			else
				break;
		else
			break;
	}

	TestY--;

	DiagNum = 0;
	while (true)
	{
		if (TestY >= 0)
			if (IsO + 1 == XO_Buttons.at(GetNumFromGrid(TestX, TestY))->TTTIndex)
			{
				DiagNum++;
				TestY--;
			}
			else
				break;
		else
			break;
	}

	if (DiagNum >= Diagonal)
		return true;

	// Проверка по -X +Y
	GetGridFromNum(Num, TestX, TestY);

	while (true)
	{
		if (TestX >= 0 && TestY < YNumber)
			if (IsO + 1 == XO_Buttons.at(GetNumFromGrid(TestX, TestY))->TTTIndex)
			{
				TestX--;
				TestY++;
			}
			else
				break;
		else
			break;
	}

	TestX++;
	TestY--;

	DiagNum = 0;
	while (true)
	{
		if (TestX < XNumber && TestY >= 0)
			if (IsO + 1 == XO_Buttons.at(GetNumFromGrid(TestX, TestY))->TTTIndex)
			{
				DiagNum++;
				TestX++;
				TestY--;
			}
			else
				break;
		else
			break;
	}

	if (DiagNum >= Diagonal)
		return true;
	

	return false;
}

bool GM_MP_Game::CanPlaceRegular(int Num) {
	return XO_Buttons.at(Num)->TTTIndex == 0;
}

bool GM_MP_Game::CanPlaceHorse(int Num, bool IsOponent)
{
	if (XO_Buttons.at(Num)->TTTIndex != 0)
		return false;

	int IndexToUse;
	if (IsOponent)
	{
		if (MyIndex == 1)
			IndexToUse = 2;
		else
			IndexToUse = 1;
	}
	else
		IndexToUse = MyIndex;

	bool TestBool = true;

	for(int i = 0; i < XO_Buttons.size(); i++)
		if (XO_Buttons.at(i)->TTTIndex == IndexToUse)
		{
			TestBool = false;
			break;
		}

	if (TestBool)
		return true;

	int MyX, MyY, TestX, TestY;

	GetGridFromNum(Num, MyX, MyY);

	for (int i = 0; i < XO_Buttons.size(); i++)
	{
		GetGridFromNum(i, TestX, TestY);
		if (abs((TestX - MyX) * (TestY - MyY)))
			if (XO_Buttons.at(i)->TTTIndex == IndexToUse)
				return true;
	}

	return false;
}
