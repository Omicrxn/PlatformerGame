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
#include "EntityManager.h"
#include "Command.h"
#include "InputHandler.h"
#include "Player.h"
#include "EnemyFly.h"
#include "PathFinding.h"

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
	app->map->Enable();
	app->map->Load("level1.tmx");
	app->entityman->Enable();

	// Create walkability map on map loading
	if(app->map->Load("level1.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		
		if(app->map->CreateWalkabilityMap(w, h, &data)) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	debugTex = app->tex->Load("Assets/maps/pathfinding_debug.png");

	player = (Player*)app->entityman->CreateEntity(EntityType::PLAYER);
	flyingEnemy1 = (EnemyFly*)app->entityman->CreateEntity(EntityType::ENEMY_FLY);

	// Load music
	app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	// Debug pathfing
	static iPoint origin;
	static bool originSelected = false;

	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);
	iPoint p = app->render->ScreenToWorld(mouseX, mouseY);
	p = app->map->WorldToMap(p.x, p.y);

	if(app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if(originSelected == true)
		{
			app->pathfinding->lastPath.Clear();

			app->pathfinding->CreatePath(origin, p);
			originSelected = false;
		}
		else
		{
			origin = p;
			originSelected = true;
		}
	}

	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	ListItem<Command*>* command = app->inputHandler->commandList.start;
	while (command != NULL) {
		command->data->Execute(player);
		command = command->next;
	}
	
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

	    // Make the camera movement independent of framerate
	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= floor(100*dt);

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += floor(100*dt);

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= floor(100*dt);

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += floor(100*dt);

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
	
	if (player->Died()) 
	{
		app->fade->Fade(this, (Module*)app->endingScreen, 180);		
	}

	// L03: DONE 7: Set the window title with map/tileset info
	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);
	/*iPoint mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x, mouseY - app->render->camera.y);

	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:[%d,%d]",
		app->map->data.width, app->map->data.height,
		app->map->data.tileWidth, app->map->data.tileHeight,
		app->map->data.tilesets.Count(), mouseTile.x, mouseTile.y);

	app->win->SetTitle(title.GetString());*/

	// L12b: Debug pathfinding
	app->input->GetMousePosition(mouseX, mouseY);
	iPoint p = app->render->ScreenToWorld(mouseX, mouseY);
	p = app->map->WorldToMap(p.x, p.y);
	p = app->map->MapToWorld(p.x, p.y);

	app->render->DrawTexture(debugTex, p.x, p.y);

	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();

	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(debugTex, pos.x, pos.y);
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

	app->entityman->Disable();
	app->map->Disable();
	app->tex->UnLoad(background1);
	app->tex->UnLoad(background2);
	app->tex->UnLoad(background3);
	app->tex->UnLoad(background4);

	app->audio->StopMusic();
	
	debugDraw = false;
	player = nullptr;
	flyingEnemy1 = nullptr;
	return true;
}