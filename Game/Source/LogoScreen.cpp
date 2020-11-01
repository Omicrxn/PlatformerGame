#include "LogoScreen.h"
#include "TitleScreen.h"
#include "App.h"
#include "Textures.h"
#include "FadeToBlack.h"
#include "Render.h"
#include "Input.h"
#include "Audio.h"
#include "Window.h"
#include "Log.h"

LogoScreen::LogoScreen(bool startEnabled) : Module(startEnabled)
{
	name = "logoscreen";
}

LogoScreen::~LogoScreen() {}

bool LogoScreen::Start()
{
	bool ret = true;

	startTime = SDL_GetTicks();
	actualTime = 0;
	endTime = 3000;
	app->win->GetWindowSize(windowSizeX, windowSizeY);
	windowScale = app->win->GetScale();

	// Screen rect
	logo = { 0,0,(int)windowSizeX * windowScale,(int)windowSizeY * windowScale };
	screen = { 0, 0, (int)windowSizeX * windowScale , (int)windowSizeY * windowScale };
	logoTex = app->tex->Load("Assets/textures/Logo Screen.png");

	return ret;
}

bool LogoScreen::Update(float dt)
{
	bool ret = true;

	if (actualTime >= endTime)
	{
		app->fade->Fade(this, (Module*)app->titleScreen, 100);
	}

	return ret;
}

bool LogoScreen::PostUpdate()
{
	bool ret = true;

	actualTime = SDL_GetTicks() - startTime;

	// Blit 
	if (!app->render->DrawTexture(logoTex, 0, 0, &logo))
	{
		ret = false;
	}

	return ret;
}

bool LogoScreen::CleanUp()
{
	bool ret = true;

	startTime = 0;
	actualTime = 0;
	endTime = 0;

	if (!app->tex->UnLoad(logoTex))
	{
		LOG("Logo Screen -> Error unloading the texture.");
		ret = false;
	}

	return ret;
}