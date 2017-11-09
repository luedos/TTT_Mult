#include "stdafx.h"
#include "GameInclude/GM_SP_Game.h"
#include <sstream>
#include "Game.h"

GM_SP_Game::GM_SP_Game(Game* InGame, Graphics* InGraph)
{
	MyGame = InGame;
	MyGraph = InGraph;

	ChangeGrid(5, 5, 3);
}


GM_SP_Game::~GM_SP_Game()
{
}

void GM_SP_Game::GM_EventTick(float DeltaTime)
{
}

void GM_SP_Game::GM_Start()
{
	XWins = 0;
	OWins = 0;

	NewRound();
}

void GM_SP_Game::NewRound()
{
	MyGraph->ClearEverything();

	XO_Buttons.clear();

	Coordinates TestCoord;
	TestCoord.bRelativeX = true;
	TestCoord.bRelativeY = true;
	TestCoord.bRelativeW = true;
	TestCoord.bRelativeH = true;

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
	TestCoord.X = 0.2;
	TestCoord.H = 60;
	TestCoord.W = 0.6;
	TestCoord.Y = 0.01;



	SomeSomeString = "X - " + to_string(XWins) + "  :  O - " + to_string(OWins);


	MyGraph->AddStaticText(SomeSomeString.c_str(), { 225,225,225,225 }, &TestCoord);


	TestCoord.Y = 0.8;

	MyGraph->AddButton(MyGame, &Game::LoadMainMenu, "Exit Game", &TestCoord);

}

void GM_SP_Game::GM_End()
{
}

void GM_SP_Game::GM_Event(SDL_Event * EventRef)
{
	if (EventRef->type == SDL_MOUSEBUTTONUP)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		for(int i = 0; i < XO_Buttons.size(); i++)
			if (XO_Buttons.at(i)->MyRect.x < x &&
				XO_Buttons.at(i)->MyRect.y < y &&
				XO_Buttons.at(i)->MyRect.x + XO_Buttons.at(i)->MyRect.w > x &&
				XO_Buttons.at(i)->MyRect.y + XO_Buttons.at(i)->MyRect.h > y)
			{
				bool TestBoolLocal = false;

				switch (MyPlaceRule)
				{
				case EPlaceRule::Regular:
				{
					TestBoolLocal = CanPlaceRegular(i);
					break;
				}
				case EPlaceRule::Horse:
				{
					TestBoolLocal = CanPlaceHorse(i);
					break;
				}
				default:
				{
					break;
				}
				}
			



				if(TestBoolLocal)
				{
					if (IsX_Turn)
						XO_Buttons.at(i)->TTTIndex = 1;
					else
						XO_Buttons.at(i)->TTTIndex = 2;

					if (CanWinCheck(i))
						WinMenu();
						
					IsX_Turn = !IsX_Turn;

					break;

				}
			}
	}
}

void GM_SP_Game::WinMenu()
{
	Coordinates TestCoord;
	TestCoord.bRelativeW = true;
	TestCoord.bRelativeX = true;
	TestCoord.bRelativeY = true;

	TestCoord.X = 0.2;
	TestCoord.W = 0.6;
	TestCoord.Y = 0.3;
	TestCoord.H = 60;



	const char* CharToRender;
	if (IsX_Turn)
	{
		++XWins;
		CharToRender = "X Wins";
	}
	else
	{
		++OWins;
		CharToRender = "O Wins";
	}


	MyGraph->AddStaticText(CharToRender, { 0,225,0,225 }, &TestCoord, 2);

	TestCoord.Y = 0.5;

	MyGraph->AddButton(this, &GM_SP_Game::NewRound, "New Round", &TestCoord, { 70,70,70,180 }, { 30,30,30,200 }, { 50,50,50,200 }, 2);

}

bool GM_SP_Game::CanWinCheck(int Num)
{
	
	int TestX, TestY;

	int MyIndex;

	if (IsX_Turn)
		MyIndex = 1;
	else
		MyIndex = 2;

	// XO_Buttons.at(Num)->TTTIndex

	// Проверка по +Х
	GetGridFromNum(Num, TestX, TestY);

	while (true)
	{
		if (TestX < XNumber)
			if (MyIndex == XO_Buttons.at(GetNumFromGrid(TestX, TestY))->TTTIndex)
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
			if (MyIndex == XO_Buttons.at(GetNumFromGrid(TestX, TestY))->TTTIndex)
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
			if (MyIndex == XO_Buttons.at(GetNumFromGrid(TestX, TestY))->TTTIndex)
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
			if (MyIndex == XO_Buttons.at(GetNumFromGrid(TestX, TestY))->TTTIndex)
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
			if (MyIndex == XO_Buttons.at(GetNumFromGrid(TestX, TestY))->TTTIndex)
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
			if (MyIndex == XO_Buttons.at(GetNumFromGrid(TestX, TestY))->TTTIndex)
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
			if (MyIndex == XO_Buttons.at(GetNumFromGrid(TestX, TestY))->TTTIndex)
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
			if (MyIndex == XO_Buttons.at(GetNumFromGrid(TestX, TestY))->TTTIndex)
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

bool GM_SP_Game::CanPlaceRegular(int Num) {
	return XO_Buttons.at(Num)->TTTIndex == 0;
}

bool GM_SP_Game::CanPlaceHorse(int Num)
{
	if (XO_Buttons.at(Num)->TTTIndex != 0)
		return false;

	int MyIndex;

	if (IsX_Turn)
		MyIndex = 1;
	else
		MyIndex = 2;

	bool TestBool = true;

	for(int i = 0; i < XO_Buttons.size(); i++)
		if (XO_Buttons.at(i)->TTTIndex == MyIndex)
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
			if (XO_Buttons.at(i)->TTTIndex == MyIndex)
				return true;
	}

	return false;
}
