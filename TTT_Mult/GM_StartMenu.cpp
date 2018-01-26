#include "stdafx.h"
#include "Game.h"
#include "GameInclude/GM_StartMenu.h"

GM_StartMenu::GM_StartMenu(Game * InGame, Graphics * InGraph)
{
	MyGraph = InGraph;
	MyGame = InGame;

	HeaderTextColor = { 225, 0 , 0, 225 };

	r = 225;
	g = 0;
	b = 0;
	ColorRate = 150;
}

void GM_StartMenu::GM_EventTick(float DeltaTime)
{
	if (r == 225)
	{
		if (b > 0)
		{
			b -= DeltaTime * ColorRate * 0.001;
			if (b < 0)
				b = 0;
		}
		else
		{
			g += DeltaTime * ColorRate * 0.001;
			if (g >= 225)
				g = 225;
		}
	}
	if (g == 225)
	{
		if (r > 0)
		{
			r -= DeltaTime * ColorRate * 0.001;
			if (r < 0)
				r = 0;
		}
		else
		{
			b += DeltaTime * ColorRate * 0.001;
			if (b > 225)
				b = 225;
		}
	}
	if (b == 225)
	{
		if (g > 0)
		{
			g -= DeltaTime * ColorRate * 0.001;
			if (g < 0)
				g = 0;
		}
		else
		{
			r += DeltaTime * ColorRate * 0.001;
			if (r > 225)
				r = 225;
		}
	}

	HeaderTextColor = { Uint8(r), Uint8(g), Uint8(b), 225 };
}

void GM_StartMenu::GM_Start()
{
	MyGraph->ClearEverything();

	MyIPButton = nullptr;
	MyPORTButton = nullptr;

	Coordinates TestCoord;
	TestCoord.bRelativeX = true;
	TestCoord.bRelativeW = true;
	TestCoord.bRelativeY = true;

	TestCoord.X = 0.05;
	TestCoord.W = 0.8;
	TestCoord.Y = 0.05;
	TestCoord.H = 50;

	MyGraph->AddDynamicText("Tic Tac Toe", &HeaderTextColor, &TestCoord);

	TestCoord.X = 0.1;
	TestCoord.W = 0.8;
	TestCoord.Y = 0.2;

	MyGraph->AddButton(this, &GM_StartMenu::ChooseSPMenu, "One PC", &TestCoord);
	
	TestCoord.Y = 0.4;

	MyGraph->AddButton(this, &GM_StartMenu::ChooseMPMenu, "Multiplayer", &TestCoord);

	TestCoord.Y = 0.6;

	MyGraph->AddButton(MyGame, &Game::ExitGame, "ExitGame", &TestCoord);

}

void GM_StartMenu::GM_End()
{
}

void GM_StartMenu::GM_Event(SDL_Event * EventRef)
{
	if (MyIPButton != nullptr)
		MyIPButton->ButtonEvent(EventRef);

	if (MyPORTButton != nullptr)
		MyPORTButton->ButtonEvent(EventRef);
}

void GM_StartMenu::ChooseSPMenu()
{
	MyGraph->ClearEverything();

	Coordinates TestCoord;
	TestCoord.bRelativeX = true;
	TestCoord.bRelativeW = true;
	TestCoord.bRelativeY = true;

	TestCoord.X = 0.05;
	TestCoord.W = 0.8;
	TestCoord.Y = 0.05;
	TestCoord.H = 60;

	MyGraph->AddDynamicText("One PC", &HeaderTextColor, &TestCoord);

	TestCoord.X = 0.1;
	TestCoord.Y = 0.2;

	RenThing_Button_DDMenu* TestDD = MyGraph->AddDDMenu("Grid", &TestCoord);
	TestDD->AddDDButton(&MyGame->SPGameplay, &GM_SP_Game::ChangeGrid3x3, "3x3 line 3");
	TestDD->AddDDButton(&MyGame->SPGameplay, &GM_SP_Game::ChangeGrid7x7, "7x7 line 4");
	TestDD->AddDDButton(&MyGame->SPGameplay, &GM_SP_Game::ChangeGrid11x11, "11x11 line 5");
	TestDD->AddDDButton(&MyGame->SPGameplay, &GM_SP_Game::ChangeGrid15x15, "15x15 line 6");
	
	TestCoord.Y = 0.4;

	RenThing_Button_DDMenu* TestDDRule = MyGraph->AddDDMenu("Rule", &TestCoord);
	TestDDRule->AddDDButton(&MyGame->SPGameplay, &GM_SP_Game::ChangeRuleRegular, "Regular");
	TestDDRule->AddDDButton(&MyGame->SPGameplay, &GM_SP_Game::ChangeRuleHorse, "Horse");

	TestCoord.Y = 0.6;


	MyGraph->AddButton(MyGame, &Game::LoadSPGameplay, "Start Game", &TestCoord);

	TestCoord.Y = 0.8;

	MyGraph->AddButton(this, &GM_StartMenu::GM_Start, "Back", &TestCoord);
}

void GM_StartMenu::ChooseMPMenu()
{
	MyGraph->ClearEverything();
	
	Coordinates TestCoord;
	TestCoord.bRelativeX = true;
	TestCoord.bRelativeY = true;

	TestCoord.X = 0.05;
	TestCoord.W = 60;
	TestCoord.Y = 0.05;
	TestCoord.H = 60;

	MyGraph->AddDynamicText("Multiplayer", &HeaderTextColor, &TestCoord);

	TestCoord.X = 0.1;
	TestCoord.Y = 0.55;

	MyGraph->AddStaticText("Is Server", { 225,225,225,225 }, &TestCoord);

	TestCoord.X = 0.7;
	
	MyGraph->AddCheckBox(MyGame->MPGameplay.IsServer, &TestCoord);

	TestCoord.bRelativeW = true;
	TestCoord.W = 0.8;
	TestCoord.X = 0.1;
	TestCoord.Y = 0.15;

	RenThing_Button_DDMenu* TestDD = MyGraph->AddDDMenu("Grid", &TestCoord);
	TestDD->AddDDButton(&MyGame->MPGameplay, &GM_MP_Game::ChangeGrid3x3, "3x3 line 3");
	TestDD->AddDDButton(&MyGame->MPGameplay, &GM_MP_Game::ChangeGrid7x7, "7x7 line 4");
	TestDD->AddDDButton(&MyGame->MPGameplay, &GM_MP_Game::ChangeGrid11x11, "11x11 line 5");
	TestDD->AddDDButton(&MyGame->MPGameplay, &GM_MP_Game::ChangeGrid15x15, "15x15 line 6");

	TestCoord.Y = 0.3;

	TestDD = MyGraph->AddDDMenu("Rule", &TestCoord);
	TestDD->AddDDButton(&MyGame->MPGameplay, &GM_MP_Game::ChangeRuleRegular, "Regular");
	TestDD->AddDDButton(&MyGame->MPGameplay, &GM_MP_Game::ChangeRuleHorse, "Horse");

	TestCoord.Y = 0.45;

	MyIPButton = MyGraph->AddInputButton("Public IP", &TestCoord);
	MyIPButton->InType = IT_NumbersAndMarks;
	MyIPButton->StringToUse = MyGame->MPGameplay.IP;

	TestCoord.Y = 0.65;

	MyPORTButton = MyGraph->AddInputButton("Target PORT", &TestCoord);
	MyPORTButton->InType = IT_Numbers;
	if(MyGame->MPGameplay.PORT != 0)
		MyPORTButton->StringToUse = to_string(MyGame->MPGameplay.PORT);
	TestCoord.Y = 0.75;

	MyGraph->AddButton(this, &GM_StartMenu::StartMP, "Start", &TestCoord);

	TestCoord.Y = 0.85;

	MyGraph->AddButton(this, &GM_StartMenu::GM_Start, "Back", &TestCoord);

}

void GM_StartMenu::StartMP()
{
	if(MyIPButton != nullptr)
		MultIP = MyIPButton->GetString();

	if (MyPORTButton != nullptr)
		MultPORT = atoi(MyPORTButton->GetString().c_str());

	MyGame->MPGameplay.LoadData(&MultIP, MultPORT);

	MyGame->LoadMPGameplay();
}
