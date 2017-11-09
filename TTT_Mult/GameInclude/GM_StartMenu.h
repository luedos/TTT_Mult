#pragma once
#include "GameMode.h"

class GM_StartMenu : public GameMode
{
public:
	GM_StartMenu(class Game* InGame, Graphics* InGraph);

	void GM_EventTick(float DeltaTime) override;

	void GM_Start() override;

	void GM_End() override;

	void GM_Event(SDL_Event* EventRef) override;

	void ChooseSPMenu();
	void ChooseMPMenu();
	
	void StartMP();

private:
	int r, g, b, ColorRate;
	SDL_Color HeaderTextColor;

	int MultPORT = 1111;
	string MultIP = "127.0.0.1";

	RenThing_Button_Input* MyIPButton = nullptr;
	RenThing_Button_Input* MyPORTButton = nullptr;
};
