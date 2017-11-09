#pragma once
#include "SDL.h"
#include "../GraphicIncludes/Graphics.h"

enum EPlaceRule
{
	Regular,
	Horse
};

class GameMode
{

public:
	GameMode() {}

	virtual void GM_EventTick(float DeltaTime) {}

	virtual void GM_Start() {}

	virtual void GM_End() {}
	
	virtual void GM_Event(SDL_Event* EventRef) {}

	Graphics* MyGraph;

	class Game* MyGame;
};
