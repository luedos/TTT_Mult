#pragma once
#include "RenThing_Button.h"




class RenThing_Button_CheckBox : public RenThing_Button
{

public:

	RenThing_Button_CheckBox(bool &BoolToCheck, SDL_Color* BGColorRef, 
		SDL_Color* BGColor_PressedRef, SDL_Color* BGColor_HoveredRef, 
		Coordinates* InCoordinates, int* ResolXRef, int* ResolYRef, int LevelRenderRef = 1) : MyBool(BoolToCheck)
	{

		MyCoordinates = *InCoordinates;
		XResol = ResolXRef;
		YResol = ResolYRef;

		LevelRender = LevelRenderRef;

		BGColor_Idle = *BGColorRef;

		BGColor_Hovered = *BGColor_HoveredRef;

		BGColor_Pressed = *BGColor_PressedRef;

		TextRect.x = 0;
		TextRect.y = 0;
	
		TextRect.w = 128;
		TextRect.h = 128;
	}

	void CleanupThing() override {
		SDL_DestroyTexture(BackgroundTexture);
		SDL_DestroyTexture(MyTexture);
	}

	void RenderThing(SDL_Renderer* RenRef) override {

		SDL_RenderCopy(RenRef, BackgroundTexture, NULL, &MyRect);

		SDL_RenderCopy(RenRef, MyTexture, &TextRect, &MyRect);

	}

	void PrepareThing(SDL_Renderer* RenRef) override {

		SDL_Color ColorToUse = { 225,225,225,225 };

		ResetCoord();


		if (BackgroundTexture == nullptr)
			BackgroundTexture = IMG_LoadTexture(RenRef, "../TTT_Mult/Textures/CheckBox_Box.png");

		if (MyTexture == nullptr)
			MyTexture = IMG_LoadTexture(RenRef, "../TTT_Mult/Textures/CheckBox_Checker.png");

		TextRect.x = MyBool * 128;
		
		SDL_SetTextureColorMod(BackgroundTexture, BGColor->r, BGColor->g, BGColor->b);

		SDL_SetTextureAlphaMod(BackgroundTexture, BGColor->a);

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

	void ButtonPressed() override {
		IsPressed = true;
	}

	void ButtonHovered(int x, int y) override {
		if (!IsPressed && MyRect.x < x && MyRect.y < y && MyRect.x + MyRect.w > x && MyRect.y + MyRect.h > y)
			BGColor = &BGColor_Hovered;
		else
			if (IsPressed)
				BGColor = &BGColor_Pressed;
			else
				BGColor = &BGColor_Idle;
		
		

	}

	void ButtonUnpressed(int x, int y, bool IsOnThis) override {

		IsPressed = false;

		if (IsOnThis)
			MyBool = !MyBool;
	}



	private:

		bool &MyBool;

};