#pragma once
#include "RenThing.h"


class RenThing_TTTButton : public RenThing
{
public:
	RenThing_TTTButton(const Coordinates* InCoord, SDL_Renderer* Ren, int* ResolXRef, int* ResolYRef, int InLevel) {
		MyCoordinates = *InCoord;

		LevelRender = InLevel;

		XResol = ResolXRef;
		YResol = ResolYRef;

		MyTexture = IMG_LoadTexture(Ren, "../TTT_Mult/Textures/TTT_XO.png");

		TTTIndex = 0;
	}

	void CleanupThing() override {	SDL_DestroyTexture(MyTexture);	}

	void RenderThing(SDL_Renderer* RenRef) override {
		SDL_Rect TestRect;
		TestRect.h = 64;
		TestRect.w = 64;
		TestRect.y = 0;
		TestRect.x = TTTIndex * 64;

		SDL_RenderCopy(RenRef, MyTexture, &TestRect, &MyRect);
	}

	void PrepareThing(SDL_Renderer* RenRef) override {
		ResetCoord();

	}

	void SetPosition(int x, int y) override {
		int LocalX = x;
		int LocalY = y;


		if (MyCoordinates.bRelativeX)
			MyCoordinates.X = float(LocalX) / *XResol;
		else
			MyCoordinates.X = LocalX;

		if (MyCoordinates.bRelativeY)
			MyCoordinates.Y = float(LocalY) / *YResol;
		else
			MyCoordinates.Y = LocalY;
	}

	int TTTIndex;

	

};


