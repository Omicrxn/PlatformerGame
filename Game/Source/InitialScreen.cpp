#include "InitialScreen.h"
#include "App.h"
#include "Textures.h"
#include "FadeToBlack.h"
#include "Render.h"
#include "Input.h"
#include "Audio.h"
#include "Window.h"
#include "Log.h"

InitialScreen::InitialScreen() : Module() {
	name = "Initial S";
	
}

InitialScreen::~InitialScreen() {}

bool InitialScreen::Start() {
	bool ret = true;

	startTime = SDL_GetTicks();
	actualTime = 0;
	endTime = 3000;
	app->win->GetWindowSize(windowSizeX, windowSizeY);
	windowScale = app->win->GetScale();
	// Screen rect
	logo = { 0,0,(int)windowSizeX * windowScale,(int)windowSizeY * windowScale };
	screen = { 0, 0, (int)windowSizeX * windowScale , (int)windowSizeY * windowScale };
	logoTex = app->tex->Load("Assets/textures/Logo.png");
	tex = app->tex->Load("Assets/textures/test.png");



	if (tex == nullptr) {
		ret = false;
	}

	return ret;
}

bool InitialScreen::Update(float dt) {
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		app->fade->Fade(this, (Module*)app->scene, 60);
	}

	return ret;
}

bool InitialScreen::PostUpdate() {
	bool ret = true;

	actualTime = SDL_GetTicks() - startTime;
	
	// Blit 
	if (actualTime < endTime) {
		if (!app->render->DrawTexture(logoTex, 0, 0, &logo)) {
			ret = false;
		}
	}
	else {
		if (!app->render->DrawTexture(tex, 0, 0, &screen, 0.0f, false,true)) {
			ret = false;
		}
	}
	
	return ret;
}

bool InitialScreen::CleanUp() {
	bool ret = true;

	startTime = 0;
	actualTime = 0;
	endTime = 0;

	if (!app->tex->UnLoad(tex)) {
		LOG("%s","Start Screen -> Error unloading the texture.");
		ret = false;
	}

	if (!app->tex->UnLoad(logoTex)) {
		LOG("Start Screen -> Error unloading the texture.");
		ret = false;
	}


	return ret;
}