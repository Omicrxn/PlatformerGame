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
#include "Coin.h"
#include "Checkpoint.h"
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
	background1 = app->tex->Load("Assets/Maps/background1.png");
	background2 = app->tex->Load("Assets/Maps/background2.png");
	background3 = app->tex->Load("Assets/Maps/background3.png");
	background4 = app->tex->Load("Assets/Maps/background4a.png");
	backgroundRect = { 0,0,2880,1440 };

	// Load map
	app->collisions->Enable();
	app->map->Enable();
	app->map->Load("level1.tmx");
	app->audio->Enable();
	app->entityman->Enable();

	// Create walkability map on map loading
	if(app->map->Load("level1.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		
		if(app->map->CreateWalkabilityMap(w, h, &data)) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	debugTex = app->tex->Load("Assets/Maps/pathfinding_debug.png");

	player = (Player*)app->entityman->CreateEntity(EntityType::PLAYER);
	walkingEnemy1 = (EnemyWalk*)app->entityman->CreateEntity(EntityType::ENEMY_WALK);
	flyingEnemy1 = (EnemyFly*)app->entityman->CreateEntity(EntityType::ENEMY_FLY);
	flyingEnemy2 = (EnemyFly*)app->entityman->CreateEntity(EntityType::ENEMY_FLY);
	flyingEnemy1->position = { 200, 800 };
	flyingEnemy2->position = { 700, 900 };
	coin = (Coin*)app->entityman->CreateEntity(EntityType::ITEM_COIN);
	heart = (Heart*)app->entityman->CreateEntity(EntityType::ITEM_HEART);
	
	checkpoint1 = (Checkpoint*)app->entityman->CreateEntity(EntityType::CHECKPOINT);
	checkpoint2 = (Checkpoint*)app->entityman->CreateEntity(EntityType::CHECKPOINT);
	checkpoint3 = (Checkpoint*)app->entityman->CreateEntity(EntityType::CHECKPOINT);
	checkpoint1->position = { 944,1328 };
	checkpoint2->position = { 1328,944 };
	checkpoint3->position = { 1936,224 };

	// Load music
	app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

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
	while (command != NULL) 
	{
		command->data->Execute(player);
		command = command->next;
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
	
	if (player->Died()) 
	{
		app->fade->Fade(this, (Module*)app->endingScreen, 180);		
	}

	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);

	// L03: DONE 7: Set the window title with map/tileset info
	/*iPoint mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x, mouseY - app->render->camera.y);

	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:[%d,%d]",
		app->map->data.width, app->map->data.height,
		app->map->data.tileWidth, app->map->data.tileHeight,
		app->map->data.tilesets.Count(), mouseTile.x, mouseTile.y);

	app->win->SetTitle(title.GetString());*/

	// L12b: Debug pathfinding
	/*if (app->pathfinding->debug == true)
	{
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
	}*/

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

	app->collisions->Disable();
	app->entityman->Disable();
	app->map->Disable();
	app->audio->StopMusic();
	app->audio->Disable();
	app->tex->Unload(background1);
	app->tex->Unload(background2);
	app->tex->Unload(background3);
	app->tex->Unload(background4);
	
	debugDraw = false;
	player = nullptr;
	flyingEnemy1 = nullptr;

	return true;
}

void Scene::DrawDebug()
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

void Scene::VolumeUp()
{
	app->audio->volume += 16;
	app->audio->VolumeChange(app->audio->volume);
}

void Scene::VolumeDown()
{
	app->audio->volume -= 16;
	app->audio->VolumeChange(app->audio->volume);
}

void Scene::UpdateCheckpoint(iPoint newPosition)
{
	if (!checkpoint3->passedCheckpoint)
		player->currentCheckpoint = newPosition;
	else if (!checkpoint2->passedCheckpoint)
		player->currentCheckpoint = newPosition;
	else if (!checkpoint1->passedCheckpoint)
		player->currentCheckpoint = newPosition;
}