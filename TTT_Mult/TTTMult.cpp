#include "stdafx.h"
#include "SDL2/include/SDL.h"
#include "SDL2/include/SDL_image.h"
#include "SDL2/include/SDL_ttf.h"
#include "SDL2/include/SDL_mixer.h"
#include "Game.h"
#include <iostream>
#include <chrono>



int main(int argc, char* argv[])
{

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "SDL_Init Error:" << SDL_GetError() << std::endl;
		return 1;
	}

	if (TTF_Init() != 0)
	{
		std::cout << "IMG_Init Error:" << IMG_GetError() << std::endl;
		return 3;
	}


	float InTime = 1.f;
	Game MyGame;

	MyGame.NewGame();

	while (!MyGame.Quit)
	{
		auto BeginTime = chrono::high_resolution_clock::now();

		MyGame.EventTick(InTime / 1000000.f);

		auto EndTime = chrono::high_resolution_clock::now();
		InTime = chrono::duration_cast<chrono::nanoseconds>(EndTime - BeginTime).count();
	}

	MyGame.ExitGame();
	SDL_Quit();


    return 0;
}

