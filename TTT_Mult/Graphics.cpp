#include "stdafx.h"
#include "GraphicIncludes/Graphics.h"


Graphics::Graphics()
{	
}


Graphics::~Graphics()
{
}

void Graphics::ChangeWindowSize(int x, int y)
{
	X_Resolution = x;
	Y_Resolution = y;
	SDL_SetWindowSize(MyWindow, x, y);

	BackgroundRect.w = x;
	BackgroundRect.h = y;

	if(!bIsFullscreen)
	SDL_SetWindowPosition(MyWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

}
void Graphics::ChangeWindowMode(bool IsFullscreen)
{
	bIsFullscreen = IsFullscreen;

	if(IsFullscreen)
	SDL_SetWindowFullscreen(MyWindow, SDL_WINDOW_FULLSCREEN);
	else
	{
		SDL_SetWindowFullscreen(MyWindow, 0);
		SDL_SetWindowPosition(MyWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}
}

void Graphics::RenderEverything(float DeltaTime)
{
	// Очистка рендера
	SDL_RenderClear(MyRenderer);

	// Если текстура фона отсутствует, загрузить текстуру
	if(BackGroundTexture == nullptr)
		BackGroundTexture = IMG_LoadTexture(MyRenderer, "../TTT_Mult/Textures/Back.png");


	// Рендер фона
	SDL_RenderCopy(MyRenderer, BackGroundTexture, NULL, &BackgroundRect);

	// Подготавка каждой штуки в трех массивах
	
	for (int i = 0; i < RestThings.size(); i++)
		RestThings.at(i)->PrepareThing(MyRenderer);

	for (int i = 0; i < ButtonsArray.size(); i++)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		ButtonsArray.at(i)->ButtonHovered(x, y);
		ButtonsArray.at(i)->PrepareThing(MyRenderer);
	}

	for (int i = 0; i < 3; i++)
		for (int ii = 0; ii < RenderOrder.size(); ii++)
			if (RenderOrder.at(ii)->LevelRender == i)
				RenderOrder.at(ii)->RenderThing(MyRenderer);



	// Ренедер рендера
	SDL_RenderPresent(MyRenderer);

}

void Graphics::ClearEverything()
{
	
	for (int i = 0; i < RestThings.size(); i++)
	{
		RestThings.at(i)->CleanupThing();
		delete RestThings.at(i);
	}

	RestThings.clear();

	for (int i = 0; i < ButtonsArray.size(); i++)
	{
		ButtonsArray.at(i)->CleanupThing();
		delete ButtonsArray.at(i);
	}

	RenderOrder.clear();

	ButtonsArray.clear();
}

void Graphics::DestroyEveryThing()
{
	ClearEverything();
	SDL_DestroyTexture(BackGroundTexture);
	SDL_DestroyRenderer(MyRenderer);
	SDL_DestroyWindow(MyWindow);
}

int Graphics::CreateGraphWindow(int WindowSizeX, int WindowSizeY)
{
	MyWindow = SDL_CreateWindow("Tic Tac Toe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowSizeX, WindowSizeY, SDL_WINDOW_SHOWN);
	if (MyWindow == nullptr)
	{
		std::cout << "SDL_CreateWindow Error:" << SDL_GetError() << std::endl;
		return 4;
	}

	MyRenderer = SDL_CreateRenderer(MyWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (MyRenderer == nullptr)
	{
		std::cout << "SDL_CreateRenderer Error:" << SDL_GetError() << std::endl;
		return 5;
	}

	X_Resolution = WindowSizeX;
	Y_Resolution = WindowSizeY;

	BackgroundRect.x = 0;
	BackgroundRect.y = 0;
	BackgroundRect.w = WindowSizeX;
	BackgroundRect.h = WindowSizeY;

	RenderEverything(1);

	return 0;
}

bool Graphics::DeleteRenThing(RenThing * ThingToDelete)
{
	bool bFind = false;

	for(int i = 0; i < RenderOrder.size(); i++)
		if (RenderOrder.at(i) == ThingToDelete)
		{
			RenderOrder.erase(RenderOrder.begin() + i);
			break;
		}

	if (!bFind)
		for (int i = 0; i < RestThings.size(); i++)
			if (RestThings.at(i) == ThingToDelete)
			{
				RestThings.at(i)->CleanupThing();
				delete RestThings.at(i);
				RestThings.erase(RestThings.begin() + i);
				bFind = true;
				break;
			}
	if(!bFind)
		for(int i = 0; i < ButtonsArray.size(); i++)
			if (ButtonsArray.at(i) == ThingToDelete)
			{
				ButtonsArray.at(i)->CleanupThing();
				delete ButtonsArray.at(i);
				ButtonsArray.erase(ButtonsArray.begin() + i);
				bFind = true; 
				break;
			}

	return bFind;
}
