#pragma once
#ifndef __LOGOSCREEN_H__
#define __LOGOSCREEN_H__

#include "Module.h"
#include "SDL/include/SDL_timer.h"
#include "SDL/include/SDL_rect.h"

struct SDL_Texture;

class LogoScreen : public Module
{
public:

	LogoScreen(bool startEnabled);
	virtual ~LogoScreen();

	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

private:

	SDL_Texture* logoTex = nullptr;
	SDL_Rect logo;
	SDL_Rect screen;

	uint windowSizeX;
	uint windowSizeY;
	int windowScale;

	Uint32 startTime = 0;
	Uint32 endTime = 0;
	Uint32 actualTime = 0;
};
#endif