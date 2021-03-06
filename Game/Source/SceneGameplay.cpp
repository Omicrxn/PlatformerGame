#include "SceneGameplay.h"

#include "EntityManager.h"
#include "Pathfinding.h"
#include "Window.h"

#include "SDL_mixer/include/SDL_mixer.h"

enum class MenuSelection
{
	NONE,
	RESUME,
	SETTINGS,
	TITLE,
	EXIT
};

enum class SettingsSelection
{
	NONE,
	MUSICVOLUME,
	FXVOLUME,
	FULLSCREEN,
	VSYNC
};

SceneGameplay::SceneGameplay::SceneGameplay(App* app, SceneManager* sceneManager, Window* win, AudioManager* audio)
{
	name = "Gameplay";

	this->app = app;
	this->sceneManager = sceneManager;
	this->window = win;
	this->audio = audio;

	// GUI: Initialize required controls for the screen
	btnResume = new GuiButton(1, { (int)win->GetWindowWidth() / 2 - 190 / 2, (int)win->GetWindowHeight() / 2 + 20, 190, 40 }, "RESUME");
	btnResume->SetObserver(this);

	btnSettings = new GuiButton(2, { (int)win->GetWindowWidth() / 2 - 190 / 2, (int)win->GetWindowHeight() / 2 + 80, 190, 40 }, "CONTINUE");
	btnSettings->SetObserver(this);

	btnTitle = new GuiButton(3, { (int)win->GetWindowWidth() / 2 - 190 / 2, (int)win->GetWindowHeight() / 2 + 140, 190, 40 }, "SETTINGS");
	btnTitle->SetObserver(this);

	btnExit = new GuiButton(4, { (int)win->GetWindowWidth() / 2 - 190 / 2, (int)win->GetWindowHeight() / 2 + 200, 190, 40 }, "CREDITS");
	btnExit->SetObserver(this);

	// GUI: Initialize required controls for the settings
	sldrMusicVolume = new GuiSlider(5, { 700, (int)win->GetWindowHeight() / 2 + 23, 35, 35 }, "MUSICVOLUME");
	sldrMusicVolume->SetObserver(this);

	sldrFxVolume = new GuiSlider(6, { 700, (int)win->GetWindowHeight() / 2 + 83, 35, 35 }, "FXVOLUME");
	sldrFxVolume->SetObserver(this);

	cbxFullscreen = new GuiCheckBox(7, { (int)win->GetWindowWidth() / 2 - 45 / 2 - 200, (int)win->GetWindowHeight() / 2 + 140, 45, 49 }, "FULLSCREEN");
	cbxFullscreen->SetObserver(this);

	cbxVSync = new GuiCheckBox(8, { (int)win->GetWindowWidth() / 2 - 45 / 2 - 200, (int)win->GetWindowHeight() / 2 + 200, 45, 49 }, "VSYNC");
	cbxVSync->SetObserver(this);

	barRect = { 0,0,300,35 };

	mouseCursorRect[0] = { 30,482,30,30 };
	mouseCursorRect[1] = { 60,482,30,30 };

	menuCurrentSelection = MenuSelection::NONE;
	settingsCurrentSelection = SettingsSelection::NONE;

	debugCP = false;
	checked = false;

	timer = 0;
}

SceneGameplay::~SceneGameplay()
{}

bool SceneGameplay::Load(Textures* tex, EntityManager* entityManager)
{
	this->entityManager = entityManager;
	
	// Load Background
	background1 = tex->Load("Assets/Maps/background1.png");
	background2 = tex->Load("Assets/Maps/background2.png");
	background3 = tex->Load("Assets/Maps/background3.png");
	background4 = tex->Load("Assets/Maps/background4a.png");
	backgroundRect = { 0,0,2880,1440 };
	menuRect = { 0,0,1280,720 };
	
	map = new Map(tex);

	// Load GUI assets
	barTexture = tex->Load("Assets/Textures/UI/bar.png");
	atlasGUITexture = tex->Load("Assets/Textures/UI/uipack_rpg_sheet.png");

	hoverFx = audio->LoadFx("Assets/Audio/Fx/bong.ogg");
	clickFx = audio->LoadFx("Assets/Audio/Fx/click.ogg");

	btnResume->SetTexture(atlasGUITexture);
	btnSettings->SetTexture(atlasGUITexture);
	btnTitle->SetTexture(atlasGUITexture);
	btnExit->SetTexture(atlasGUITexture);
	sldrMusicVolume->SetTexture(atlasGUITexture);
	sldrFxVolume->SetTexture(atlasGUITexture);
	cbxFullscreen->SetTexture(atlasGUITexture);
	cbxVSync->SetTexture(atlasGUITexture);

	heartAnimation.PushBack({ 0,0,16,16 });
	heartAnimation.PushBack({ 16,0,16,16 });
	heartAnimation.PushBack({ 32,0,16,16 });
	heartAnimation.PushBack({ 48,0,16,16 });
	heartAnimation.PushBack({ 64,0,16,16 });
	heartAnimation.PushBack({ 80,0,16,16 });
	heartAnimation.PushBack({ 96,0,16,16 });
	heartAnimation.PushBack({ 112,0,16,16 });
	heartAnimation.loop = false;
	heartAnimation.speed = 0.09f;

	coinAnimation.PushBack({ 0,0,16,16 });
	coinAnimation.PushBack({ 16,0,16,16 });
	coinAnimation.PushBack({ 32,0,16,16 });
	coinAnimation.PushBack({ 48,0,16,16 });
	coinAnimation.PushBack({ 63,0,16,16 });
	coinAnimation.loop = false;
	coinAnimation.speed = 0.09f;

	// Load HUD assets
	heartTexture = tex->Load("Assets/Textures/Entities/Items/heart.png");
	heartRect = { 0,0,16,16 };
	coinTexture = tex->Load("Assets/Textures/Entities/Items/coin.png");
	coinRect = { 0,0,16,16 };

	// L03: DONE: Load map
	// L12b: Create walkability map on map loading
	if (map->Load("platformer.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;

		if (map->CreateWalkabilityMap(w, h, &data)) PathFinding::GetInstance()->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	// Load font
	font = new Font("Assets/Fonts/happy_school.xml", tex);

	// Load game entities
	// Player load
	player = (Player*)entityManager->CreateEntity(EntityType::PLAYER);
	player->SetTexture(tex->Load("Assets/Textures/Entities/Player/player_sheet.png"));

	// Checkpoint load
	for (int i = 0; i < MAX_CHECKPOINTS; ++i)
	{
		checkpoints[i] = (Checkpoint*)entityManager->CreateEntity(EntityType::CHECKPOINT);
		checkpoints[i]->SetTexture(tex->Load("Assets/Textures/Entities/Checkpoints/checkpoint_statue.png"));

		// Assign player to checkpoints
		checkpoints[i]->SetPlayer(player);
	}
	checkpoints[0]->position = iPoint(19 * 64, 1572);
	checkpoints[1]->position = iPoint(36*64, 16*64+27);

	// Item load
	for (int i = 0; i < MAX_COINS; ++i)
	{
		coins[i] = (Coin*)entityManager->CreateEntity(EntityType::COIN);
		coins[i]->SetTexture(tex->Load("Assets/Textures/Entities/Items/coin.png"));
		
		// Assign player to coins
		coins[i]->SetPlayer(player);

		// Assign scene to coins
		coins[i]->SetScene(this);
	}
	coins[0]->position = iPoint(21 * 64+16, 39 * 64);

	for (int i = 0; i < MAX_HEARTS; ++i)
	{
		hearts[i] = (Heart*)entityManager->CreateEntity(EntityType::HEART);
		hearts[i]->SetTexture(tex->Load("Assets/Textures/Entities/Items/heart.png"));

		// Assign player to hearts
		hearts[i]->SetPlayer(player);

		// Assign scene to hearts
		hearts[i]->SetScene(this);
	}
	hearts[0]->position = iPoint(6 * 64, 23 * 64);

	// Flying enemy load
	for (int i = 0; i < MAX_FLYING_ENEMIES; ++i)
	{
		enemiesFly[i] = (EnemyFly*)entityManager->CreateEntity(EntityType::ENEMYFLY);
		enemiesFly[i]->SetTexture(tex->Load("Assets/Textures/Entities/Enemies/enemy_fly.png"), tex->Load("Assets/Maps/path_debug.png"));

		// Assign player and map to flying enemies
		enemiesFly[i]->SetPlayer(player);
		enemiesFly[i]->SetMap(map);
	}
	enemiesFly[0]->position = iPoint(15 * 64, 33 * 64);

	// Walking enemy load

	for (int i = 0; i < MAX_WALKING_ENEMIES; ++i)
	{
		enemiesWalk[i] = (EnemyWalk*)entityManager->CreateEntity(EntityType::ENEMYWALK);
		enemiesWalk[i]->SetTexture(tex->Load("Assets/Textures/Entities/Enemies/slime.png"), tex->Load("Assets/Maps/path_debug.png"));

		// Assign player and map to walking enemies
		enemiesWalk[i]->SetPlayer(player);
		enemiesWalk[i]->SetMap(map);
	}
	enemiesWalk[0]->position = iPoint(20 * 64, -10 * 64);

	//if (sceneManager->continueOption)
	//{
	//	app->LoadGameRequest();
	//	//sceneManager->continueOption = false;
	//}
	
    return false;
}

inline bool CheckCollision(SDL_Rect rec1, SDL_Rect rec2)
{
	if ((rec1.x < (rec2.x + rec2.w) && (rec1.x + rec1.w) > rec2.x) &&
		(rec1.y < (rec2.y + rec2.h) && (rec1.y + rec1.h) > rec2.y)) return true;
	else return false;
}

bool SceneGameplay::Update(Input* input, Collisions* collisions, float dt)
{
	if (!pause)
	{
		// Collision detection: map vs player
		player->Update(input, dt);
		if (player->dead)
		{
			TransitionToScene(SceneType::TITLE);
		}
		CollisionHandler();

		// Debug Keys
		// Request Save / Load
		if (input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
			app->SaveGameRequest();
		if (input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
			app->LoadGameRequest();

		// Move between the different checkpoints
		if (input->GetKey(SDL_SCANCODE_F7) == KeyState::KEY_DOWN)
		{
			if (!debugCP)
			{
				player->position.x = checkpoints[0]->position.x - player->width;
				player->position.y = checkpoints[0]->position.y;
				debugCP = true;
			}
			else
			{
				player->position.x = checkpoints[1]->position.x - player->width;
				player->position.y = checkpoints[1]->position.y;
				debugCP = false;
			}
		}

		// View colliders
		if (input->GetKey(SDL_SCANCODE_F9) == KeyState::KEY_DOWN)
		{
			map->drawColliders = !map->drawColliders;
			collisions->debug = !collisions->debug;

			for (int i = 0; i < MAX_FLYING_ENEMIES; ++i)
			{
				enemiesFly[i]->debugDraw = !enemiesFly[i]->debugDraw;
			}
			for (int i = 0; i < MAX_WALKING_ENEMIES; ++i)
			{
				enemiesWalk[i]->debugDraw = !enemiesWalk[i]->debugDraw;
			}
		}

		// God Mode
		if (input->GetKey(SDL_SCANCODE_F10) == KeyState::KEY_DOWN)
		{
			player->godMode = !player->godMode;
		}

		// Pause
		if (input->GetKey(SDL_SCANCODE_ESCAPE) == KeyState::KEY_DOWN)
		{
			pause = !pause;
			ListItem<Entity*>* entity = entityManager->entities.start;
			while (entity != NULL)
			{
				entity->data->pause = !entity->data->pause;
				entity = entity->next;
			}
		}

		timer += dt;
	}
	else
	{
		if (menuCurrentSelection == MenuSelection::NONE)
		{
			btnResume->Update(input, dt, audio, hoverFx, clickFx);
			btnSettings->Update(input, dt, audio, hoverFx, clickFx);
			btnTitle->Update(input, dt, audio, hoverFx, clickFx);
			btnExit->Update(input, dt, audio, hoverFx, clickFx);
		}
		else if (menuCurrentSelection == MenuSelection::RESUME)
		{
			menuCurrentSelection = MenuSelection::NONE;
			pause = !pause;
			ListItem<Entity*>* entity = entityManager->entities.start;
			while (entity != NULL)
			{
				entity->data->pause = !entity->data->pause;
				entity = entity->next;
			}
		}
		else if (menuCurrentSelection == MenuSelection::SETTINGS)
		{
			sldrMusicVolume->Update(input, dt);
			sldrFxVolume->Update(input, dt);
			cbxFullscreen->Update(input, dt);
			cbxVSync->Update(input, dt);

			if (settingsCurrentSelection == SettingsSelection::MUSICVOLUME)
			{
				Mix_VolumeMusic(sldrMusicVolume->GetValue());
			}
			else if (settingsCurrentSelection == SettingsSelection::FXVOLUME)
			{
				Mix_Volume(-1, sldrFxVolume->GetValue());
			}
			else if (settingsCurrentSelection == SettingsSelection::FULLSCREEN)
			{
				if (fullscreen == false)
				{
					SDL_SetWindowFullscreen(window->window, 1);
					fullscreen = true;
				}
				else
				{
					SDL_SetWindowFullscreen(window->window, 0);
					fullscreen = false;
				}
				settingsCurrentSelection = SettingsSelection::NONE;
			}
			else if (settingsCurrentSelection == SettingsSelection::VSYNC)
			{
				if (vsync == false)
				{
					SDL_GL_SetSwapInterval(1);
					vsync = true;
				}
				else
				{
					SDL_GL_SetSwapInterval(0);
					vsync = false;
				}
			}
		}
		else if (menuCurrentSelection == MenuSelection::TITLE)
		{
			TransitionToScene(SceneType::TITLE);
		}
		else if (menuCurrentSelection == MenuSelection::EXIT)
		{
			sceneManager->menuExitCall = true;
		}

		if (input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
			menuCurrentSelection = MenuSelection::NONE;
	}

	input->GetMousePosition(mousePos.x, mousePos.y);

	if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
		clicking = true;
	else
		clicking = false;

	if (!checked)
	{
		for (int i = 0; i < MAX_CHECKPOINTS; ++i)
		{
			if (player->lastCheckpointPos.x == checkpoints[i]->position.x - player->width && player->lastCheckpointPos.y == checkpoints[i]->position.y)
				checkpoints[i]->achieved = true;
			else
				player->lastCheckpointPos = iPoint(96, 2367);
		}
		checked = true;
	}

	return true;
}

bool SceneGameplay::Draw(Render* render)
{
	render->CameraUpdate(player->position);
	DrawBackground(render);

	// Draw map
	map->Draw(render);

	if (!pause)
	{
		if (animateHeart)
		{
			animateHeart = false;
			heartAnimation.Reset();
		}
		if (animateCoin)
		{
			animateCoin = false;
			coinAnimation.Reset();
		}
		render->scale = 3;
		if (heartAnimation.Finished())
		{
			render->DrawTextureWithoutCamera(heartTexture, 25, 25, &heartRect);
		}
		else
		{
			SDL_Rect heart = heartAnimation.GetCurrentFrame();
			render->DrawTextureWithoutCamera(heartTexture, 25, 25, &heart, 1.0f);
		}
		if (coinAnimation.Finished())
		{
			render->DrawTextureWithoutCamera(coinTexture, 500, 25, &coinRect);
		}
		else
		{
			SDL_Rect coin = coinAnimation.GetCurrentFrame();
			render->DrawTextureWithoutCamera(coinTexture, 500, 25, &coin, 1.0f);
		}
		render->scale = 1;

		int offset = 3;

		char lifes[16] = { 0 };
		sprintf_s(lifes, 16, "%03i", player->lifes);
		render->DrawText(font, lifes, 95 + offset, 30 + offset, 40, 5, { 105,105,105,255 });
		render->DrawText(font, lifes, 95, 30, 40, 5, { 255,255,255,255 });

		char coins[16] = { 0 };
		sprintf_s(coins, 16, "%03i", player->score);
		render->DrawText(font, coins, 575 + offset, 30 + offset, 40, 5, { 105,105,105,255 });
		render->DrawText(font, coins, 575, 30, 40, 5, { 255,255,255,255 });

		char time[16] = { 0 };
		sprintf_s(time, 16, "Time: %03d", (int)timer);
		render->DrawText(font, time, 1075 + offset, 25 + offset, 40, 5, { 105,105,105,255 });
		render->DrawText(font, time, 1075, 25, 40, 5, { 255,255,255,255 });
	}
	else
	{
		render->DrawRectangleWithoutCamera(menuRect, { 0,0,0,127 });

		if (menuCurrentSelection == MenuSelection::NONE)
		{
			btnResume->Draw(render);
			btnSettings->Draw(render);
			btnTitle->Draw(render);
			btnExit->Draw(render);

			int offset = 3;
			render->DrawText(font, "PAUSE", 500 + offset, 250 + offset, 100, 13, { 105,105,105,255 });
			render->DrawText(font, "PAUSE", 500, 250, 100, 13, { 255,255,255,255 });

			render->DrawText(font, "RESUME", 575 + offset, 383 + offset, 40, 5, { 105,105,105,255 });
			render->DrawText(font, "SETTINGS", 557 + offset, 443 + offset, 40, 5, { 105,105,105,255 });
			render->DrawText(font, "TITLE", 595 + offset, 503 + offset, 40, 5, { 105,105,105,255 });
			render->DrawText(font, "EXIT", 605 + offset, 563 + offset, 40, 5, { 105,105,105,255 });

			render->DrawText(font, "RESUME", 575, 383, 40, 5, { 255,255,255,255 });
			render->DrawText(font, "SETTINGS", 557, 443, 40, 5, { 255,255,255,255 });
			render->DrawText(font, "TITLE", 595, 503, 40, 5, { 255,255,255,255 });
			render->DrawText(font, "EXIT", 605, 563, 40, 5, { 255,255,255,255 });
		}
		else if (menuCurrentSelection == MenuSelection::SETTINGS)
		{
			render->DrawTextureWithoutCamera(barTexture, 567, 383, &barRect);
			render->DrawTextureWithoutCamera(barTexture, 567, 443, &barRect);

			sldrMusicVolume->Draw(render);
			sldrFxVolume->Draw(render);
			cbxFullscreen->Draw(render);
			cbxVSync->Draw(render);

			int offset = 3;
			render->DrawText(font, "SETTINGS", 425 + offset, 250 + offset, 100, 13, { 105,105,105,255 });
			render->DrawText(font, "SETTINGS", 425, 250, 100, 13, { 255,255,255,255 });

			render->DrawText(font, "MUSIC", 425 + offset, 383 + offset, 40, 5, { 105,105,105,255 });
			render->DrawText(font, "MUSIC", 425, 383, 40, 5, { 255,255,255,255 });
			render->DrawText(font, "FX", 425 + offset, 443 + offset, 40, 5, { 105,105,105,255 });
			render->DrawText(font, "FX", 425, 443, 40, 5, { 255,255,255,255 });
			render->DrawText(font, "FULLSCREEN", 475 + offset, 503 + offset, 40, 5, { 105,105,105,255 });
			render->DrawText(font, "FULLSCREEN", 475, 503, 40, 5, { 255,255,255,255 });
			render->DrawText(font, "VSYNC", 475 + offset, 563 + offset, 40, 5, { 105,105,105,255 });
			render->DrawText(font, "VSYNC", 475, 563, 40, 5, { 255,255,255,255 });

			render->DrawText(font, "PRESS 'B' TO RETURN", 475 + offset, 623 + offset, 40, 5, { 105,105,105,255 });
			render->DrawText(font, "PRESS 'B' TO RETURN", 475, 623, 40, 5, { 255,255,255,255 });
		}

		if (clicking)
			render->DrawTextureWithoutCamera(atlasGUITexture, mousePos.x, mousePos.y, &mouseCursorRect[0]);
		else
			render->DrawTextureWithoutCamera(atlasGUITexture, mousePos.x, mousePos.y, &mouseCursorRect[1]);
	}
	
    return false;
}

bool SceneGameplay::Unload(Textures* tex, AudioManager* audio)
{
	// TODO: Unload all resources
	tex->UnLoad(background1);
	tex->UnLoad(background2);
	tex->UnLoad(background3);
	tex->UnLoad(background4);

	map->CleanUp();
	RELEASE(map);
	tex->UnLoad(barTexture);
	tex->UnLoad(atlasGUITexture);

	tex->UnLoad(heartTexture);
	tex->UnLoad(coinTexture);

	RELEASE(font);
	player->active = false;

	// GUI: Initialize required controls for the screen
	RELEASE(btnResume);

	RELEASE(btnSettings);

	RELEASE(btnTitle);

	RELEASE(btnExit);

	// GUI: Initialize required controls for the settings
	RELEASE(sldrMusicVolume);

	RELEASE(sldrFxVolume);

	RELEASE(cbxFullscreen);

	RELEASE(cbxVSync);

	PathFinding::GetInstance()->CleanUp();
    return true;
}

void SceneGameplay::CollisionHandler() 
{
	// Check if updated player position collides with next tile
	// IMPROVEMENT: Just check adyacent tiles to player
	ListItem<Entity*>* entity = entityManager->entities.start;
	while (entity != NULL)
	{
		for (int y = 0; y < map->data.height; y++)
		{
			for (int x = 0; x < map->data.width; x++)
			{
				//Check ground
				if ((map->data.layers[4]->Get(x, y) >= 86) &&
					CheckCollision(map->GetTilemapRec(x, y), entity->data->GetBounds()))
				{
					if (entity->data->type == EntityType::PLAYER)
					{
						player->readyToJump = true;
					}

					if (entity->data->type != EntityType::ENEMYFLY)
					{
						entity->data->position = entity->data->tempPosition;
						entity->data->velocity.y = 0.0f;
					}

					break;
				}

				//Check water
				if ((map->data.layers[4]->Get(x, y) == 85) &&
					CheckCollision(map->GetTilemapRec(x, y), entity->data->GetBounds()))
				{
					if (entity->data->type == EntityType::PLAYER)
					{
						player->Die();
					}

					if (entity->data->type != EntityType::ENEMYFLY)
						entity->data->velocity.y = 0.0f;

					break;
				}
			}
		}
		entity = entity->next;
	}
}

void SceneGameplay::DrawBackground(Render* render)
{
	render->scale = 1.5;
	render->DrawTexture(background1, 0, 150, &backgroundRect, 0.2f);
	render->DrawTexture(background2, 0, 150, &backgroundRect, 0.4f);
	render->DrawTexture(background3, 0, 150, &backgroundRect, 0.6f);
	render->DrawTexture(background4, 0, 150, &backgroundRect, 0.8f);
	render->scale = 1;
}

//----------------------------------------------------------
// Manage GUI events for this screen
//----------------------------------------------------------
bool SceneGameplay::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1) menuCurrentSelection = MenuSelection::RESUME;
		else if (control->id == 2) menuCurrentSelection = MenuSelection::SETTINGS;
		else if (control->id == 3) menuCurrentSelection = MenuSelection::TITLE;
		else if (control->id == 4) menuCurrentSelection = MenuSelection::EXIT;
	}
	case GuiControlType::SLIDER:
	{
		if (control->id == 5) settingsCurrentSelection = SettingsSelection::MUSICVOLUME;
		else if (control->id == 6) settingsCurrentSelection = SettingsSelection::FXVOLUME;
	}
	case GuiControlType::CHECKBOX:
	{
		if (control->id == 7) settingsCurrentSelection = SettingsSelection::FULLSCREEN;
		else if (control->id == 8) settingsCurrentSelection = SettingsSelection::VSYNC;
	}
	default: break;
	}

	return true;
}