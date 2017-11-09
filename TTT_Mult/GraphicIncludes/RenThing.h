#pragma once
#include "SDL.h"
#include "SDL_image.h"


struct Coordinates
{
	bool bRelativeX = false;
	bool bRelativeY = false;
	bool bRelativeW = false;
	bool bRelativeH = false;
	float X;
	float Y;
	float W;
	float H;
};

class RenThing

{
	
public:

	RenThing() {}
	
	virtual void CleanupThing() {}

	virtual void RenderThing(SDL_Renderer* RenRef) {}

	virtual void PrepareThing(SDL_Renderer* RenRef) {}

	virtual void SetPosition(int x, int y) {}

	virtual void ResetCoord() {

		if (MyCoordinates.bRelativeX)
			MyRect.x = *XResol * MyCoordinates.X;
		else
			MyRect.x = MyCoordinates.X;

		if (MyCoordinates.bRelativeY)
			MyRect.y = *YResol * MyCoordinates.Y;
		else
			MyRect.y = MyCoordinates.Y;

		if (MyCoordinates.bRelativeW)
			MyRect.w = *XResol * MyCoordinates.W;
		else
			MyRect.w = MyCoordinates.W;

		if (MyCoordinates.bRelativeH)
			MyRect.h = *YResol * MyCoordinates.H;
		else
			MyRect.h = MyCoordinates.H;
	}

	void InterpPosition(int InterpTo_x, int InterpTo_y, float DeltaTime_MS, float PercentInSec)
	{
		SetPosition(MyRect.x + float(InterpTo_x - MyRect.x) * DeltaTime_MS * 0.001 * PercentInSec,
			MyRect.y + float(InterpTo_y - MyRect.y) * DeltaTime_MS * 0.001 * PercentInSec);
	}

	SDL_Rect MyRect;

	Coordinates MyCoordinates;

	SDL_Texture* MyTexture = nullptr;

	bool FirstCreated = true;

	int LevelRender;

	int* XResol;
	int* YResol;
};

