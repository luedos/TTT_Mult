#pragma once
#include "GameMode.h"



class GM_SP_Game :	public GameMode
{
public:

	string SomeSomeString;

	GM_SP_Game(class Game* InGame, Graphics* InGraph);
	~GM_SP_Game();

	void GM_EventTick(float DeltaTime) override;

		void GM_Start() override;

		void NewRound();

		void GM_End() override;

		void GM_Event(SDL_Event* EventRef) override;

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
		
		void ChangeGrid3x3() { ChangeGrid(3, 3, 3); }
		void ChangeGrid7x7() { ChangeGrid(7, 7, 4); }
		void ChangeGrid11x11() { ChangeGrid(11, 11, 5); }
		void ChangeGrid15x15() { ChangeGrid(15, 15, 6); }

		EPlaceRule MyPlaceRule = EPlaceRule::Regular;

private:

	int GetNumFromGrid(int X, int Y) { return Y * XNumber + X; }

	void GetGridFromNum(int Num, int& X, int& Y) {
		Y = Num / XNumber;
		X = Num - Y * XNumber;
	}

	void WinMenu();

	bool CanWinCheck(int Num);

	bool CanPlaceRegular(int Num);

	bool CanPlaceHorse(int Num);

	bool IsX_Turn;

		int XNumber;
		int YNumber;

		int Diagonal;

		SDL_Rect GridRect;

		vector<RenThing_TTTButton*> XO_Buttons;

		int XWins, OWins;
};

