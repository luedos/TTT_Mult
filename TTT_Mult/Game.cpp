#include "stdafx.h"
#include "Game.h"


Game::Game()
{

}


Game::~Game()
{
}

void Game::EventTick(float DeltaTime)
{
	bool NotSkip = true;

	if (GM_NowPlaying == &MPGameplay)
		if (MPGameplay.MyMultiplayer != nullptr)
		if (MPGameplay.MyMultiplayer->IsProcessing)
			NotSkip = false;

	if (NotSkip)
	{
		while (SDL_PollEvent(&MyEvent))
		{
			SDL_PumpEvents();

			GM_NowPlaying->GM_Event(&MyEvent);

			if (MyEvent.type == SDL_QUIT)
			{
				ExitGame();
			}
			if (MyEvent.type == SDL_MOUSEBUTTONDOWN)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);

				RenThing_Button* TestButton = FindButton(x, y);
				if (TestButton != nullptr)
					TestButton->ButtonPressed();
			}
			if (MyEvent.type == SDL_MOUSEBUTTONUP)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);

				RenThing_Button* TestButton = FindButton(x, y);
				if (TestButton != nullptr)
					TestButton->ButtonUnpressed(x, y, true);

				for (int i = 0; i < GameGraphics.ButtonsArray.size(); i++)
					if (GameGraphics.ButtonsArray.at(i) != TestButton)
						GameGraphics.ButtonsArray.at(i)->ButtonUnpressed(x, y, false);
			}
		}
	}

	if (GM_NowPlaying == &MPGameplay)
		if(MPGameplay.MyMultiplayer != nullptr)
		if (MPGameplay.MyMultiplayer->IsProcessing)
			NotSkip = false;

	if (NotSkip && GM_NowPlaying != nullptr)
	{
		GM_NowPlaying->GM_EventTick(DeltaTime);
	}

	if (GM_NowPlaying == &MPGameplay)
		if (MPGameplay.MyMultiplayer != nullptr)
		if (MPGameplay.MyMultiplayer->IsProcessing)
			NotSkip = false;

	if (NotSkip)
	{
		GameGraphics.RenderEverything(DeltaTime);
	}
}

void Game::NewGame()
{
	GameGraphics.CreateGraphWindow(480, 640);

	GM_NowPlaying = &MainMenu;
	GM_NowPlaying->GM_Start();
}

void Game::ExitGame()
{
	GM_NowPlaying->GM_End();

	GameGraphics.DestroyEveryThing();

	Quit = true;
}


RenThing_Button* Game::FindButton(int x, int y)
{
	SDL_Rect* TestRect;

	RenThing_Button* TestButton = nullptr;
	int TestRenLevel = 0;

	for (int i = GameGraphics.ButtonsArray.size() - 1; i > -1; i--)
	{
		TestRect = &GameGraphics.ButtonsArray.at(i)->MyRect;

		if (TestRect->x < x && TestRect->y < y && TestRect->x + TestRect->w > x && TestRect->y + TestRect->h > y && GameGraphics.ButtonsArray.at(i)->LevelRender >= TestRenLevel)
		{
			TestButton = GameGraphics.ButtonsArray.at(i);
			TestRenLevel = GameGraphics.ButtonsArray.at(i)->LevelRender;
		}
	}

	return TestButton;
}