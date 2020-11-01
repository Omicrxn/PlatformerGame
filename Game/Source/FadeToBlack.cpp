#include "FadeToBlack.h"

#include "App.h"
#include "Render.h"
#include "Window.h"

#include "SDL/include/SDL_render.h"

#include "Log.h"

FadeToBlack::FadeToBlack(bool startEnabled) : Module( startEnabled)
{
	name = "fadetoblack";
}

FadeToBlack::~FadeToBlack() {}
	
bool FadeToBlack::Start()
{
	LOG("%s","Preparing Fade Screen");
	app->win->GetWindowSize(windowSizeX, windowSizeY);
	windowScale = app->win->GetScale();
	screenRect = { 0, 0, (int)windowSizeX * windowScale , (int)windowSizeY * windowScale };
	moduleToEnable = nullptr;
	moduleToDisable = nullptr;

	// Enable blending mode for transparency
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	
	return true;
}

bool FadeToBlack::Update(float dt)
{
	// Exit this function if we are not performing a fade
	if (currentStep == FadeStep::NONE) return true;

	if (currentStep == FadeStep::TO_BLACK)
	{
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			moduleToDisable->Disable();
			moduleToEnable->Enable();

			currentStep = FadeStep::FROM_BLACK;
		}
	}
	else
	{
		--frameCount;
		if (frameCount <= 0)
		{
			currentStep = FadeStep::NONE;
		}
	}

	return true;
}

bool FadeToBlack::PostUpdate()
{
	// Exit this function if we are not performing a fade
	if (currentStep == FadeStep::NONE) return true;

	float fadeRatio = (float)frameCount / (float)maxFadeFrames;

	// Render the black square with alpha on the screen
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));

	SDL_RenderFillRect(app->render->renderer, &screenRect);

	return true;
}

bool FadeToBlack::Fade(Module* moduleToDisable, Module* moduleToEnable, float frames)
{
	bool ret = false;

	// If we are already in a fade process, ignore this call
	if (currentStep == FadeStep::NONE)
	{
		currentStep = FadeStep::TO_BLACK;
		frameCount = 0;
		maxFadeFrames = frames;

		this->moduleToDisable = moduleToDisable;
		this->moduleToEnable = moduleToEnable;

		ret = true;
	}

	return ret;
}

bool FadeToBlack::hasEnded() 
{
	bool ret = true;

	if (currentStep == FadeStep::NONE) ret = true;
	else ret = false;

	return ret;
}