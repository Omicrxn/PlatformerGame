#include "TitleScreen.h"
#include "App.h"
#include "Textures.h"
#include "FadeToBlack.h"
#include "Render.h"
#include "Input.h"
#include "Audio.h"
#include "Window.h"
#include "Log.h"

TitleScreen::TitleScreen() : Module()
{
	name = "TitleScreen";
}

TitleScreen::~TitleScreen() {}

bool TitleScreen::Start()
{
	bool ret = true;

	app->win->GetWindowSize(windowSizeX, windowSizeY);
	windowScale = app->win->GetScale();

	// Screen rect
	screen = { 0, 0, (int)windowSizeX * windowScale , (int)windowSizeY * windowScale };
	tex = app->tex->Load("Assets/textures/Title Screen.jpg");

	if (tex == nullptr)
	{
		ret = false;
	}

	return ret;
}

bool TitleScreen::Update(float dt)
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		app->fade->Fade(this, (Module*)app->scene, 100);
	}

	return ret;
}

bool TitleScreen::PostUpdate()
{
	bool ret = true;
	
	// Blit 
	if (!app->render->DrawTexture(tex, 0, 0, &screen, 0.0f))
	{
		ret = false;
	}
	
	return ret;
}

bool TitleScreen::CleanUp()
{
	bool ret = true;

	if (!app->tex->UnLoad(tex)) 
	{
		LOG("%s","Title Screen -> Error unloading the texture.");
		ret = false;
	}

	return ret;
}