#pragma once
#ifndef __ENDINGSCREEN_H__
#define __ENDINGSCREEN_H__

#include "Module.h"
#include "SDL/include/SDL_timer.h"
#include "SDL/include/SDL_rect.h"
struct SDL_Texture;
class EndingScreen : public Module
{
public:
	EndingScreen(bool startEnabled);
	virtual ~EndingScreen();

	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

private:
	SDL_Texture* tex = nullptr;
	SDL_Rect screen;

	uint windowSizeX;
	uint windowSizeY;
	int windowScale;
};
#endif