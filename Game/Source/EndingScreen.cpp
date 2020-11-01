#include "EndingScreen.h"
#include "App.h"
#include "Textures.h"
#include "FadeToBlack.h"
#include "Render.h"
#include "Input.h"
#include "Audio.h"
#include "Window.h"
#include "Log.h"

EndingScreen::EndingScreen(bool startEnabled) : Module(startEnabled)
{
	name = "endingscreen";
}

EndingScreen::~EndingScreen() {}

bool EndingScreen::Start()
{
	bool ret = true;
	
	app->win->GetWindowSize(windowSizeX, windowSizeY);
	windowScale = app->win->GetScale();
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	// Screen rect
	screen = { 0, 0, (int)windowSizeX * windowScale , (int)windowSizeY * windowScale };
	tex = app->tex->Load("Assets/textures/Ending Screen.jpg");

	if (tex == nullptr)
	{
		ret = false;
	}

	return ret;
}

bool EndingScreen::Update(float dt)
{
	bool ret = true;
	
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		app->fade->Fade(this, (Module*)app->titleScreen, 100);
	}

	// F1/F2 Start from the first/second level
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		app->fade->Fade(this, (Module*)app->scene, 100);
	}

	return ret;
}

bool EndingScreen::PostUpdate()
{
	bool ret = true;

	// Blit 
	if (!app->render->DrawTexture(tex, 0, 0, &screen, 0.0f))
	{
		ret = false;
	}

	return ret;
}

bool EndingScreen::CleanUp()
{
	bool ret = true;

	if (!app->tex->UnLoad(tex))
	{
		LOG("%s", "Ending Screen -> Error unloading the texture.");
		ret = false;
	}

	return ret;
}