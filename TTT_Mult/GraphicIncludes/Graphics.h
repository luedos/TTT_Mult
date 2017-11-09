#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "RenThing_Text.h"
#include "RenThing_Image.h"
#include "RenThing_Button_DDMenu.h"
#include "RenThing_TTTButton.h"
#include "RenThing_Button_Input.h"
#include "RenThing_Button_CheckBox.h"

using namespace std;

class Graphics

{
public:

	Graphics();
	~Graphics();

	RenThing_Text* AddStaticText(const char* InCharRef, SDL_Color InStatColor, Coordinates* InCoord, int RenLevel = 1) {
			RenThing_Text* Test = new RenThing_Text(InCharRef, InStatColor, InCoord, &X_Resolution, &Y_Resolution, RenLevel);
			RestThings.push_back(Test);
			RenderOrder.push_back(RestThings.back());
			return Test;
		
		
	}

	RenThing_Text* AddDynamicText(const char* InCharRef, SDL_Color* InDynColor,const Coordinates* InCoord, int RenLevel = 1) {
			RenThing_Text* Test = new RenThing_Text(InCharRef, InDynColor, *InCoord, &X_Resolution, &Y_Resolution, RenLevel);
			RestThings.push_back(Test);
			RenderOrder.push_back(RestThings.back());
			return Test;
		
	}

	RenThing_Image* AddStaticImage(char *FileName , Coordinates* InCoord,SDL_Rect* DynRect = nullptr, SDL_Color* ColorRef = nullptr, int RenLevel = 0) {
			RenThing_Image* Test = new RenThing_Image(MyRenderer, FileName, InCoord, &X_Resolution, &Y_Resolution, DynRect, ColorRef, RenLevel);
			RestThings.push_back(Test);
			RenderOrder.push_back(RestThings.back());
			return Test;
	}

	template<class T, class M>
	RenThing_Button* AddButton(T* MyClass, M MyMethod, const char* ButtonTextRef, 
		Coordinates* InCoord, SDL_Color BGColorRef = { 150,150,150,120 }, SDL_Color BGColor_PressedRef = { 100,100,100,180 },
		SDL_Color BGColor_HoveredRef = {120,120,120,150}, int RenLevel = 1) {
		
		RenThing_Button* TestTest = new RenThing_Button(ButtonTextRef, &BGColorRef, &BGColor_PressedRef, &BGColor_HoveredRef, InCoord, &X_Resolution, &Y_Resolution, RenLevel);

		TestTest->ConnectNew(MyClass, MyMethod);

		ButtonsArray.push_back(TestTest);
		RenderOrder.push_back(ButtonsArray.back());
		return ButtonsArray.back();

	}

	RenThing_Button_DDMenu* AddDDMenu(const char* ButtonTextRef,Coordinates* InCoord, SDL_Color BGColorRef = { 150,150,150,120 },
		SDL_Color BGColor_PressedRef = { 100,100,100,180 }, SDL_Color BGColor_HoveredRef = { 120,120,120,150 }, int LevelRenderRef = 1) {

		RenThing_Button_DDMenu* TestTest = new RenThing_Button_DDMenu(ButtonTextRef, &BGColorRef, &BGColor_PressedRef, &BGColor_HoveredRef, InCoord, &X_Resolution, &Y_Resolution, LevelRenderRef);

		ButtonsArray.push_back(TestTest);
		RenderOrder.push_back(ButtonsArray.back());
		return TestTest;
	}

	RenThing_TTTButton* AddTTTButton(const Coordinates* InCoord, int InLevel = 1) {
		RenThing_TTTButton* Test = new RenThing_TTTButton(InCoord, MyRenderer, &X_Resolution, &Y_Resolution, InLevel);

		RestThings.push_back(Test);
		RenderOrder.push_back(Test);

		return Test;
	}

	RenThing_Button_Input* AddInputButton(const char* ButtonTextRef, Coordinates* InCoordinates, SDL_Color BGColorRef = { 150,150,150,120 },
		SDL_Color BGColor_PressedRef = { 100,100,100,180 }, SDL_Color BGColor_HoveredRef = { 120,120,120,150 }, SDL_Color BGColor_ActiveColorRef = { 200,200,200,200 }, int LevelRenderRef = 1)
	{
		RenThing_Button_Input* TestTest = new RenThing_Button_Input(ButtonTextRef, &BGColorRef, &BGColor_PressedRef, &BGColor_HoveredRef, &BGColor_ActiveColorRef, InCoordinates, &X_Resolution, &Y_Resolution, LevelRenderRef);

		ButtonsArray.push_back(TestTest);
		RenderOrder.push_back(TestTest);

		return TestTest;
	}

	RenThing_Button_CheckBox* AddCheckBox(bool &BoolToCheck, Coordinates* InCoord, SDL_Color BGColorRef = { 150,150,150,120 }, 
		SDL_Color BGColor_PressedRef = { 100,100,100,180 },	SDL_Color BGColor_HoveredRef = { 120,120,120,150 }, int RenLevel = 1)
	{
		RenThing_Button_CheckBox* Test = new RenThing_Button_CheckBox(BoolToCheck, &BGColorRef, &BGColor_PressedRef, &BGColor_HoveredRef, InCoord, &X_Resolution, &Y_Resolution, RenLevel);

		ButtonsArray.push_back(Test);
		RenderOrder.push_back(Test);

		return Test;
	}

	void ChangeWindowSize(int x, int y);

	void ChangeWindowMode(bool IsFullscreen);

	void RenderEverything(float DeltaTime);

	void ClearEverything();

	void DestroyEveryThing();

	int CreateGraphWindow(int WindowSizeX, int WindowSizeY);

	bool DeleteRenThing(RenThing* ThingToDelete);

	vector<RenThing_Button*> ButtonsArray;

	bool bIsFullscreen = false;

	int X_Resolution, Y_Resolution;

	void Resolution800x600() { ChangeWindowSize(800, 600); }

	void Resolution1280x720() { ChangeWindowSize(1280, 720); }

	void Resolution1600x900() { ChangeWindowSize(1600, 900); }

	void WindFullscreen() { ChangeWindowMode(true); }

	void WindWindowed() { ChangeWindowMode(false); }

private:

	SDL_Renderer* MyRenderer;
	SDL_Window* MyWindow;

	vector<RenThing*> RestThings;

	SDL_Texture* BackGroundTexture;
	SDL_Rect BackgroundRect;

	vector<RenThing*> RenderOrder;
};

