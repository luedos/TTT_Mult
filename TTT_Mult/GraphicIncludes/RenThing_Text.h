#pragma once
#include "RenThing.h"
#include <string>
#include <iostream>
#include "SDL_ttf.h"

using namespace std;

class RenThing_Text : public RenThing
{
public:
	
	RenThing_Text(const char* InCharToLocate, SDL_Color NewStaticColor,
		Coordinates* InCoordinates, int* ResolXRef, int* ResolYRef, int LevelRenderRef = 1) {


		

		MyCoordinates = *InCoordinates;
		XResol = ResolXRef;
		YResol = ResolYRef;

		LevelRender = LevelRenderRef;

		UseStaticColor = true;
		StaticColor = NewStaticColor;
		SomeToRender = InCharToLocate;

		SetFont(48);

	}

	RenThing_Text(const char* InCharToLocate, SDL_Color* NewDynamicColor,
		Coordinates InCoordinates, int* ResolXRef, int* ResolYRef, int LevelRenderRef = 1) {

		MyCoordinates = InCoordinates;
		XResol = ResolXRef;
		YResol = ResolYRef;

		LevelRender = LevelRenderRef;

		UseStaticColor = false;
		DynamicColor = NewDynamicColor;
		SomeToRender = InCharToLocate;

		SetFont(48);
	}


	void RenderThing(SDL_Renderer* RenRef) override {

		SDL_RenderCopy(RenRef, MyTexture, NULL, &MyRect);
	}

	void CleanupThing() override { TTF_CloseFont(Font);
	SDL_DestroyTexture(MyTexture);
	}

	void PrepareThing(SDL_Renderer* RenRef) override {
		SDL_Color ColorToUse;
		if (UseStaticColor)
			ColorToUse = StaticColor;
		else
			ColorToUse = *DynamicColor;

		SDL_DestroyTexture(MyTexture);

		Surface = TTF_RenderText_Blended(Font, SomeToRender, ColorToUse);
		MyTexture = SDL_CreateTextureFromSurface(RenRef, Surface);
		SDL_FreeSurface(Surface);

		ResetCoord();

		SDL_QueryTexture(MyTexture, NULL, NULL, &MyRect.w, &MyRect.h);


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

	void SetFont(int ptsizeRef) { 
		TTF_CloseFont(Font);
		Font = TTF_OpenFont("../TTT_Mult/Fonts/RiseOfKingdom.ttf", ptsizeRef); 
	}

private:

	TTF_Font* Font;

	const char* SomeToRender;

	SDL_Surface* Surface;

	SDL_Color StaticColor;
	SDL_Color* DynamicColor;

	bool UseStaticColor;


};