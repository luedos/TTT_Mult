#pragma once
#include "ServerClass.h"
#include "ClientClass.h"
#include "GameMode.h"


class GM_MP_Game :	public GameMode
{
public:
	GM_MP_Game(class Game* InGame, Graphics* InGraph);
	~GM_MP_Game();

	void GM_EventTick(float DeltaTime) override;

	void GM_Start() override;

	void LoadData(string* IPRef, int PORTRef);

	void NewRound();

	void LoadForNewRound();

	void GM_End() override;

	void GM_Event(SDL_Event* EventRef) override;

	bool MakeTurn(int InIndex, bool Hard = false, bool IsOponent = false);

	void ChangeGrid(int InX, int InY, int InDiagonal) {
			XNumber = InX;
			YNumber = InY;
			
			Diagonal = InDiagonal;

			if (Diagonal > InX)
				Diagonal = InX;
			
			if (Diagonal > InY)
				Diagonal = InY;

			GridRect.w = InX * 64;
			GridRect.h = InY * 64;

		}
		
	void ChangeRuleRegular() { MyPlaceRule = EPlaceRule::Regular; }
	void ChangeRuleHorse() { MyPlaceRule = EPlaceRule::Horse; }

	EPlaceRule MyPlaceRule = EPlaceRule::Regular;

	bool IsMyTurn;

	int MyIndex;

	bool IsServer;

	MultBase* MyMultiplayer;

	const char* SecondPlayerIP;
	int SecondPlayerPORT;

	ClientClass MyClient = ClientClass(this);
	ServerClass MyServer = ServerClass(this);

	int PORT;
	string IP;
	
	void ChangeGrid3x3() { ChangeGrid(3, 3, 3); }
	void ChangeGrid7x7() { ChangeGrid(7, 7, 4); }
	void ChangeGrid11x11() { ChangeGrid(11, 11, 5); }
	void ChangeGrid15x15() { ChangeGrid(15, 15, 6); }

	bool IsMeReady, IsOponentReady;

	int XWins, OWins;

	void WinMenu(bool IsX);

	bool CanWinCheck(int Num, bool IsO);
private:

	int GetNumFromGrid(int X, int Y) { return Y * XNumber + X; }

	void GetGridFromNum(int Num, int& X, int& Y) {
		Y = Num / XNumber;
		X = Num - Y * XNumber;
	}
	


	bool CanPlaceRegular(int Num);

	bool CanPlaceHorse(int Num, bool IsOponent);

	int XNumber;
	int YNumber;

	int Diagonal;

	SDL_Rect GridRect;

	vector<RenThing_TTTButton*> XO_Buttons;

	string SomeSomeString;
};

