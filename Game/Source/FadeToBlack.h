#ifndef __FADETOBLACK_H__
#define __FADETOBLACK_H__

#include "Module.h"
#include "SDL/include/SDL_Rect.h"

class FadeToBlack : public Module
{
public:
	// Constructor
	FadeToBlack(bool startEnabled);

	// Destructor
	virtual ~FadeToBlack();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called from another module
	// Starts the fade process which has two steps, fade_out and fade_in
	// After the first step, the modules should be switched
	bool Fade(Module* toDisable, Module* toEnable, float frames = 60);

	bool hasEnded();

private:

	enum Fade_Step
	{
		NONE,
		TO_BLACK,
		FROM_BLACK
	} 
	
	currentStep = Fade_Step::NONE;

	// A frame count system to handle the fade time and ratio
	Uint32 frameCount = 0;
	Uint32 maxFadeFrames = 0;

	// The rectangle of the screen, used to render the black rectangle
	SDL_Rect screenRect;
	uint windowSizeX = 0;
	uint windowSizeY = 0;
	int windowScale = 0;

	// The modules that should be switched after the first step
	Module* moduleToEnable = nullptr;
	Module* moduleToDisable = nullptr;
};

#endif //__MODULEFADETOBLACK_H__

