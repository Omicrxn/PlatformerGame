#pragma once
#ifndef __TITLESCREEN_H__
#define __TITLESCREEN_H__

#include "Module.h"
#include "SDL/include/SDL_timer.h"
#include "SDL/include/SDL_rect.h"

struct SDL_Texture;
class TitleScreen : public Module
{
public:

	TitleScreen(bool startEnabled);

	virtual ~TitleScreen();
	
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