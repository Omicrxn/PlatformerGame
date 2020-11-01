#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "LogoScreen.h"
#include "TitleScreen.h"
#include "EndingScreen.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene(bool startEnabled) : Module(startEnabled)
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// Load Backgrounds
	background1 = app->tex->Load("Assets/maps/background1.png");
	background2 = app->tex->Load("Assets/maps/background2.png");
	background3 = app->tex->Load("Assets/maps/background3.png");
	background4 = app->tex->Load("Assets/maps/background4a.png");
	backgroundRect = { 0,0,2880,1440 };

	// Load map
	app->map->Load("level1.tmx");
	app->player->Enable();
	
	// Load music
	app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	// F5 Save the current game state
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveGameRequest();

	// F6 Load the previous state (even across levels)
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadGameRequest();

	// F9 View colliders / logic
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{ 
		debugDraw = !debugDraw;
		if (debugDraw)
		{
			app->map->ChangeLayerProperty("Collisions", "Draw", 1);
		}
		else
		{
			app->map->ChangeLayerProperty("Collisions", "Draw", 0);
		}
	}

	// Change volume with +/- from the numeric keyboard
	if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
	{
		app->audio->volume += 16;
		app->audio->VolumeChange(app->audio->volume);
	}

	if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
	{
		app->audio->volume -= 16;
		app->audio->VolumeChange(app->audio->volume);
	}

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) app->render->camera.y -= 1;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) app->render->camera.y += 1;

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) app->render->camera.x -= 1;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) app->render->camera.x += 1;

	if (!app->render->DrawTexture(background1, 0, 0, &backgroundRect, false, 0.2f))
	{
		LOG("%s %s", "Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		return false;
	}

	if (!app->render->DrawTexture(background2, 0, 0, &backgroundRect, false, 0.4f))
	{
		LOG("%s %s", "Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		return false;
	}

	if (!app->render->DrawTexture(background3, 0, 0, &backgroundRect, false, 0.6f))
	{
		LOG("%s %s", "Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		return false;
	}
	
	if (!app->render->DrawTexture(background4, 0, 0, &backgroundRect, false, 0.8f))
	{
		LOG("%s %s", "Cannot blit the texture in ModulePlayer %s\n", SDL_GetError());
		return false;
	}
	
	// Draw map
	app->map->Draw();
	
	if (app->player->Died()) 
	{
		app->fade->Fade(this, (Module*)app->endingScreen, 180);		
	}

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;
	
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	app->player->Disable();

	app->tex->UnLoad(background1);
	app->tex->UnLoad(background2);
	app->tex->UnLoad(background3);
	app->tex->UnLoad(background4);

	app->audio->StopMusic();

	debugDraw = false;

	return true;
}