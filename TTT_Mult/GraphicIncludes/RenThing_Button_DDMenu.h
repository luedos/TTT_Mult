#pragma once
#include "RenThing_Button.h"
#include <iostream>
#include <vector>
using namespace std;

class RenThing_Button_DDMenu : public RenThing_Button
{
public:

	RenThing_Button_DDMenu(const char* ButtonTextRef, 
		SDL_Color* BGColorRef, SDL_Color* BGColor_PressedRef, SDL_Color* BGColor_HoveredRef, 
		Coordinates* InCoordinates, int* ResolXRef, int* ResolYRef, int LevelRenderRef = 1) {

		MyCoordinates = *InCoordinates;
		XResol = ResolXRef;
		YResol = ResolYRef;
		
		LevelRender = LevelRenderRef;

		ResetCoord();

		MyRect = RealButtonRect;

		ButtonText = ButtonTextRef;

		BGColor_Idle = *BGColorRef;

		BGColor_Hovered = *BGColor_HoveredRef;

		BGColor_Pressed = *BGColor_PressedRef;

		OpenDDFunc(false);

		SetFont(48);
	}

	void CleanupThing() override {}

	void RenderThing(SDL_Renderer* RenRef) override {

		SDL_RenderCopy(RenRef, BackgroundTexture, NULL, &MyRect);

		SDL_RenderCopy(RenRef, MyTexture, NULL, &TextRect);

		if (OpenDD)
			for (int i = 0; i < DDButtonVector.size(); i++)
				DDButtonVector.at(i)->RenderThing(RenRef);

	}

	void PrepareThing(SDL_Renderer* RenRef) override {

		SDL_Color ColorToUse = { 225,225,225,225 };

		SDL_DestroyTexture(MyTexture);

		Surface = TTF_RenderText_Blended(Font, ButtonText, ColorToUse);
		MyTexture = SDL_CreateTextureFromSurface(RenRef, Surface);
		SDL_FreeSurface(Surface);

		SDL_QueryTexture(MyTexture, NULL, NULL, &TextRect.w, &TextRect.h);

		ResetCoord();

		TextRect.x = RealButtonRect.x + float(RealButtonRect.w - TextRect.w) / 2;

		TextRect.y = RealButtonRect.y + float(RealButtonRect.h - TextRect.h) / 2;

		if (BackgroundTexture == nullptr)
		{
			BackgroundTexture = IMG_LoadTexture(RenRef, "../TTT_Mult/Textures/SimpleWhite.png");
			if (BackgroundTexture == nullptr)
			{
				cout << "Bad Background" << endl;
				SDL_Quit();
			}
		}

		SDL_SetTextureColorMod(BackgroundTexture, BGColor->r, BGColor->g, BGColor->b);

		SDL_SetTextureAlphaMod(BackgroundTexture, BGColor->a);
		
		if(OpenDD)
			for (int i = 0; i < DDButtonVector.size(); i++)
			{
				DDButtonVector.at(i)->PrepareThing(RenRef);
				DDButtonVector.at(i)->SetPosition(RealButtonRect.x, RealButtonRect.y + RealButtonRect.h + i * RealButtonRect.h * 0.8f);
			}
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

		if (OpenDD)
			for (int i = 0; i < DDButtonVector.size(); i++)
				DDButtonVector.at(i)->ButtonHovered(x, y);
	}

	void ButtonUnpressed(int x, int y, bool IsOnThis) override{

		IsPressed = false;

		if (IsOnThis)
			if (MyRect.x < x && MyRect.y < y && MyRect.x + MyRect.w > x && MyRect.y + MyRect.h > y && OpenDD)
				for (int i = 0; i < DDButtonVector.size(); i++)
					if (DDButtonVector.at(i)->MyRect.x < x
						&& DDButtonVector.at(i)->MyRect.y < y
						&& DDButtonVector.at(i)->MyRect.x + DDButtonVector.at(i)->MyRect.w > x
						&& DDButtonVector.at(i)->MyRect.y + DDButtonVector.at(i)->MyRect.h > y)
					{
						DDButtonVector.at(i)->ButtonUnpressed(x, y, true);
						break;
					}



			if (RealButtonRect.x < x && RealButtonRect.y < y && RealButtonRect.x + RealButtonRect.w > x && RealButtonRect.y + RealButtonRect.h > y && !OpenDD && IsOnThis)
				OpenDDFunc(true);
			else
				if (OpenDD)
					OpenDDFunc(false);
		
	}

	void OpenDDFunc(bool InPoint) {
		OpenDD = InPoint;

		if (OpenDD)
			LevelRender = 2;
		else
			LevelRender = 1;
	}
	

	template<typename T, typename M>
	void AddDDButton(T* ClassRef, M MethodRef, const char* DDName) {

		Coordinates NewCoord = MyCoordinates;
		NewCoord.bRelativeX = false;
		NewCoord.bRelativeY = false;
		NewCoord.H = NewCoord.H * 0.8;
	
		SDL_Color ButtonColorIdle = BGColor_Idle;
		ButtonColorIdle.a = 256;
		SDL_Color ButtonColorPressed = BGColor_Pressed;
		ButtonColorPressed.a = 256;
		SDL_Color ButtonColorHovered = BGColor_Hovered;
		ButtonColorHovered.a = 256;

		DDButtonVector.push_back(new RenThing_Button(DDName, &ButtonColorIdle, &ButtonColorPressed, &ButtonColorHovered, &NewCoord, XResol, YResol, 2));
		DDButtonVector.back()->ConnectNew(ClassRef, MethodRef);
	}

	void ResetCoord() override {

		if (MyCoordinates.bRelativeX)
			RealButtonRect.x = *XResol * MyCoordinates.X;
		else
			RealButtonRect.x = MyCoordinates.X;

		if (MyCoordinates.bRelativeY)
			RealButtonRect.y = *YResol * MyCoordinates.Y;
		else
			RealButtonRect.y = MyCoordinates.Y;

		if (MyCoordinates.bRelativeW)
			RealButtonRect.w = *XResol * MyCoordinates.W;
		else
			RealButtonRect.w = MyCoordinates.W;

		if (MyCoordinates.bRelativeH)
			RealButtonRect.h = *YResol * MyCoordinates.H;
		else
			RealButtonRect.h = MyCoordinates.H;

		MyRect = RealButtonRect;

		if (OpenDD)
			MyRect.h = (DDButtonVector.size() * 0.8f + 1.f) * RealButtonRect.h;
		
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

	bool OpenDD;

	SDL_Rect RealButtonRect;

	RenThing_Button* ButtonInCharge;

	vector<RenThing_Button*> DDButtonVector;
	
};