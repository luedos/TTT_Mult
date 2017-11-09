#pragma once
#include "RenThing.h"


class RenThing_Image : public RenThing

{

public:


	RenThing_Image(SDL_Renderer* Ren, const char *File,
		Coordinates* InCoordinates, int* ResolXRef, int* ResolYRef, SDL_Rect* DynRect = nullptr,
		SDL_Color* ColorRef = nullptr, int LevelRenderRef = 0) {
		LevelRender = LevelRenderRef;

		

		if (DynRect == nullptr)
			MyDynRect = NULL;
		else
			MyDynRect = DynRect;

		MyCoordinates = *InCoordinates;
		XResol = ResolXRef;
		YResol = ResolYRef;

	MyTexture = IMG_LoadTexture(Ren, File);
	

	if (ColorRef != nullptr)
	{
		SDL_SetTextureColorMod(MyTexture, ColorRef->r, ColorRef->g, ColorRef->b);
		SDL_SetTextureAlphaMod(MyTexture, ColorRef->a);
	}
	}

	void PrepareThing(SDL_Renderer* RenRef) override { ResetCoord(); }

	void RenderThing(SDL_Renderer* RenRef) override { SDL_RenderCopy(RenRef, MyTexture, MyDynRect, &MyRect); }

	void CleanupThing() override { SDL_DestroyTexture(MyTexture); }

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

	SDL_Rect* MyDynRect;

};

