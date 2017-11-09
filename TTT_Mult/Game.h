#pragma once
#include "GraphicIncludes/Graphics.h"
#include "GameInclude/GM_SP_Game.h"
#include "GameInclude/GM_MP_Game.h"
#include "GameInclude/GM_StartMenu.h"



class Game
{
public:
	Game();
	~Game();

	void EventTick(float DeltaTime);

	void NewGame();

	void ExitGame();

	bool Quit = false;

	void LoadMainMenu() { 
		GM_NowPlaying->GM_End();
		GM_NowPlaying = &MainMenu;
		MainMenu.GM_Start();
	}
	void LoadSPGameplay() {
		GM_NowPlaying->GM_End();
		GM_NowPlaying = &SPGameplay;
		SPGameplay.GM_Start();
	}
	void LoadMPGameplay() {
		GM_NowPlaying->GM_End();
		GM_NowPlaying = &MPGameplay;
		MPGameplay.GM_Start();
	}

	GM_StartMenu MainMenu = GM_StartMenu(this, &GameGraphics);
	GM_SP_Game SPGameplay = GM_SP_Game(this, &GameGraphics);
	GM_MP_Game MPGameplay = GM_MP_Game(this, &GameGraphics);
private:

	RenThing_Button* FindButton(int x, int y);

	Graphics GameGraphics;

	SDL_Event MyEvent;

	GameMode* GM_NowPlaying;

};

