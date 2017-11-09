#pragma once
#include "RenThing_Text.h"





class Button_IContainer
{
public:
	virtual void Call() = 0;

};

template<class T, class M>
class Button_Container : public Button_IContainer
{};


template<class T>
class Button_Container<T, void(T::*)()> : public Button_IContainer
{

	typedef void (T::*M)();

public:

	Button_Container(T* c, M m) : MyClass(c), MyMethod(m) {}

	void Call() { (MyClass->*MyMethod)();	}

private:
	T* MyClass;
	M MyMethod;

};



class RenThing_Button : public RenThing
{

public:
	RenThing_Button() {}

	RenThing_Button(const char* ButtonTextRef, SDL_Color* BGColorRef, 
		SDL_Color* BGColor_PressedRef, SDL_Color* BGColor_HoveredRef, 
		Coordinates* InCoordinates, int* ResolXRef, int* ResolYRef, int LevelRenderRef = 1){

		MyCoordinates = *InCoordinates;
		XResol = ResolXRef;
		YResol = ResolYRef;

		LevelRender = LevelRenderRef;

		ButtonText = ButtonTextRef;

		BGColor_Idle = *BGColorRef;

		BGColor_Hovered = *BGColor_HoveredRef;

		BGColor_Pressed = *BGColor_PressedRef;

		BGColor = &BGColor_Idle;

		SetFont(48);
	
	}


	template<class T, class U>
	void ConnectNew(T* NewClass, U NewMethod)
	{
		if (MyContainer != nullptr)
			delete MyContainer;

		MyContainer = new Button_Container<T, U>(NewClass, NewMethod);
	}


	void CleanupThing() override {
		SDL_DestroyTexture(MyTexture);
		SDL_DestroyTexture(BackgroundTexture);
	}

	void RenderThing(SDL_Renderer* RenRef) override {

		SDL_RenderCopy(RenRef, BackgroundTexture, NULL, &MyRect);

		SDL_RenderCopy(RenRef, MyTexture, NULL, &TextRect);

	}

	void PrepareThing(SDL_Renderer* RenRef) override {

		SDL_Color ColorToUse = { 225,225,225,225 };

		SDL_DestroyTexture(MyTexture);

		Surface = TTF_RenderText_Blended(Font, ButtonText, ColorToUse);
		MyTexture = SDL_CreateTextureFromSurface(RenRef, Surface);
		SDL_FreeSurface(Surface);

		SDL_QueryTexture(MyTexture, NULL, NULL, &TextRect.w, &TextRect.h);

		ResetCoord();

		TextRect.x = MyRect.x + float(MyRect.w - TextRect.w) / 2;

		TextRect.y = MyRect.y + float(MyRect.h - TextRect.h) / 2;

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

	virtual void ButtonPressed() {
		IsPressed = true;
	}

	virtual void ButtonHovered(int x, int y) {
		if (!IsPressed && MyRect.x < x && MyRect.y < y && MyRect.x + MyRect.w > x && MyRect.y + MyRect.h > y)
			BGColor = &BGColor_Hovered;
		else
			if (IsPressed)
				BGColor = &BGColor_Pressed;
			else
				BGColor = &BGColor_Idle;
		
		

	}

	virtual void ButtonUnpressed(int x, int y, bool IsOnThis) {

		IsPressed = false;

		if(IsOnThis)
			if (MyContainer != nullptr)
				MyContainer->Call();		
	}

	void SetFont(int ptsizeRef) {
		TTF_CloseFont(Font);
		Font = TTF_OpenFont("../TTT_Mult/Fonts/RiseOfKingdom.ttf", ptsizeRef);
	}

	const char* ButtonText;

protected:

	TTF_Font* Font;

	SDL_Surface* Surface;

	SDL_Color* BGColor;

	SDL_Color BGColor_Idle;

	SDL_Color BGColor_Hovered;

	SDL_Color BGColor_Pressed;

	SDL_Rect TextRect;

	bool IsPressed = false;

	SDL_Texture* BackgroundTexture = nullptr;

	Button_IContainer* MyContainer = nullptr;
};