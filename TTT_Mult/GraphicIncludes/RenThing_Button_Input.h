#pragma once
#include "RenThing_Button.h"

enum E_InputType
{
	IT_Numbers,
	IT_NumbersAndMarks,
	IT_All,
	IT_Letters,
	IT_LetterAndMarks	
};

class RenThing_Button_Input : public RenThing_Button
{

public:
	RenThing_Button_Input(const char* ButtonTextRef, SDL_Color* BGColorRef, 
		SDL_Color* BGColor_PressedRef, SDL_Color* BGColor_HoveredRef, SDL_Color* BGColor_ActiveColorRef,
		Coordinates* InCoordinates, int* ResolXRef, int* ResolYRef, int LevelRenderRef = 2){

		MyCoordinates = *InCoordinates;
		XResol = ResolXRef;
		YResol = ResolYRef;

		LevelRender = LevelRenderRef;

		ButtonText = ButtonTextRef;

		BGColor_Idle = *BGColorRef;

		BGColor_Hovered = *BGColor_HoveredRef;

		BGColor_Pressed = *BGColor_PressedRef;

		BGColor_ActiveColor = *BGColor_ActiveColorRef;

		SetFont(48);
	
	}


	void CleanupThing() override {
		SDL_DestroyTexture(MyTexture);
		SDL_DestroyTexture(BackgroundTexture);
	}

	void RenderThing(SDL_Renderer* RenRef) override {

		SDL_RenderCopy(RenRef, BackgroundTexture, NULL, &MyRect);

		if(!IsPressed || !StringToUse.empty())
			SDL_RenderCopy(RenRef, MyTexture, NULL, &TextRect);

	}

	void PrepareThing(SDL_Renderer* RenRef) override {

		SDL_DestroyTexture(MyTexture);

		SDL_Color ColorToUse = { 225,225,225,225 };
		SDL_Color EmptyUse = { 150,150,150,150 };
		if(StringToUse.empty())
			Surface = TTF_RenderText_Blended(Font, ButtonText, EmptyUse);
		else
			Surface = TTF_RenderText_Blended(Font, StringToUse.c_str(), ColorToUse);

		MyTexture = SDL_CreateTextureFromSurface(RenRef, Surface);
		SDL_FreeSurface(Surface);

		SDL_QueryTexture(MyTexture, NULL, NULL, &TextRect.w, &TextRect.h);

		ResetCoord();

		TextRect.x = MyRect.x + float(MyRect.w - TextRect.w) / 2;

		TextRect.y = MyRect.y + float(MyRect.h - TextRect.h) / 2;

		if (BackgroundTexture == nullptr)
		{
			BackgroundTexture = IMG_LoadTexture(RenRef, "../TTT_Mult/Textures/SimpleWhite.png");
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

	 void ButtonPressed() override {
		BGColor = &BGColor_Pressed;
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
		if (IsOnThis)
		{
			IsPressed = true;
			BGColor = &BGColor_ActiveColor;
		}
		else
			IsPressed = false;
	}

	SDL_Color BGColor_ActiveColor;

	void ButtonEvent(SDL_Event* InEvent) {
	
		if (!IsPressed)
			return;

		if (InEvent->type != SDL_KEYDOWN)
			return;

		if (InEvent->button.button == SDL_SCANCODE_BACKSPACE && !StringToUse.empty())
		{
			StringToUse.pop_back();
			return;
		}

		switch (InType)
		{
		case IT_Letters:
		{
			LocalString = CheckForLetters(InEvent->button.button);
			if (LocalString != "NONE")
				StringToUse += LocalString;
			
			break;
		}

		case IT_LetterAndMarks:
		{
			LocalString = CheckForLetters(InEvent->button.button);
			if (LocalString != "NONE")
				StringToUse += LocalString;

			LocalString = CheckForMarks(InEvent->button.button);
			if (LocalString != "NONE")
				StringToUse += LocalString;
			
			break;
		}
		case IT_Numbers:
		{		
			LocalString = CheckForNumbers(InEvent->button.button);
			if (LocalString != "NONE")
				StringToUse += LocalString;

			break;
		}

		case IT_NumbersAndMarks:
		{
			LocalString = CheckForNumbers(InEvent->button.button);
			if (LocalString != "NONE")
				StringToUse += LocalString;

			LocalString = CheckForMarks(InEvent->button.button);
			if (LocalString != "NONE")
				StringToUse += LocalString;

			break;
		}

		case IT_All:
		{
			LocalString = CheckForLetters(InEvent->button.button);
			if (LocalString != "NONE")
				StringToUse += LocalString;

			LocalString = CheckForNumbers(InEvent->button.button);
			if (LocalString != "NONE")
				StringToUse += LocalString;

			LocalString = CheckForMarks(InEvent->button.button);
			if (LocalString != "NONE")
				StringToUse += LocalString;

			break;
		}

		default:
			break;
		}

	}

	E_InputType InType = IT_All;

	string GetString() { return StringToUse; }

	string StringToUse;

protected:

	string CheckForNumbers(Uint8 InEvent)
	{
		for (int i = 0; i < 10; ++i)
			if (NumberEvents[i] == InEvent)
				return NumberStrings[i];

		return "NONE";
	}

	string CheckForLetters(Uint8 InEvent)
	{
		for (int i = 0; i < 26; ++i)
			if (LetterEvents[i] == InEvent)
				return LetterStrings[i];

		return "NONE";
	}

	string CheckForMarks(Uint8 InEvent)
	{
		for (int i = 0; i < 10; ++i)
			if (MarkEvents[i] == InEvent)
				return MarkStrings[i];

		return "NONE";
	}

private:

	Uint8 NumberEvents[10] = { SDL_SCANCODE_0, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4,
						SDL_SCANCODE_5, SDL_SCANCODE_6, SDL_SCANCODE_7, SDL_SCANCODE_8, SDL_SCANCODE_9 };

	string NumberStrings[10] = { "0","1","2","3","4","5","6","7","8","9" };
	
	Uint8 LetterEvents[26] = { SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_R, SDL_SCANCODE_T,
		SDL_SCANCODE_Y, SDL_SCANCODE_U, SDL_SCANCODE_I, SDL_SCANCODE_O, SDL_SCANCODE_P,
		SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_F, SDL_SCANCODE_G,
		SDL_SCANCODE_H, SDL_SCANCODE_J, SDL_SCANCODE_K, SDL_SCANCODE_L, SDL_SCANCODE_Z,
		SDL_SCANCODE_X, SDL_SCANCODE_C, SDL_SCANCODE_V, SDL_SCANCODE_B, SDL_SCANCODE_N,
		SDL_SCANCODE_M };

	string LetterStrings[26] = { "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
								"A", "S", "D", "F", "G", "H", "J", "K", "L", "Z",
								"X", "C", "V", "B", "N", "M" };

	Uint8 MarkEvents[3] = { SDL_SCANCODE_PERIOD, SDL_SCANCODE_COMMA, SDL_SCANCODE_SPACE };

	string MarkStrings[3] = { ".", ",", " " };

	string LocalString;

};