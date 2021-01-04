#include "SceneGameplay.h"

#include "EntityManager.h"

SceneGameplay::SceneGameplay()
{
	name = "Gameplay";
}

SceneGameplay::~SceneGameplay()
{}

bool SceneGameplay::Load(Textures* tex, EntityManager* entityManager)
{
	//Load Background
	background1 = tex->Load("Assets/Maps/background1.png");
	background2 = tex->Load("Assets/Maps/background2.png");
	background3 = tex->Load("Assets/Maps/background3.png");
	background4 = tex->Load("Assets/Maps/background4a.png");
	backgroundRect = { 0,0,2880,1440 };
	
	map = new Map(tex);

	// L03: DONE: Load map
	// L12b: Create walkability map on map loading
	if (map->Load("platformer.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;

		//if (map->CreateWalkabilityMap(w, h, &data)) pathFinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	// Load music
	//AudioManager::PlayMusic("Assets/Audio/Music/music_spy.ogg");

	// Load game entities
	player = (Player*)entityManager->CreateEntity(EntityType::PLAYER);
	player->SetTexture(tex->Load("Assets/Textures/Entities/Player/player_sheet.png"));
	player->position = iPoint(96, 2350);
	//Item load
	coin = (Coin*)entityManager->CreateEntity(EntityType::COIN);
	coin->SetTexture(tex->Load("Assets/Textures/Entities/Items/coin.png"));
	coin->SetPlayer(player);
	coin->position = iPoint(21 * 64+16, 39 * 64);
	heart = (Heart*)entityManager->CreateEntity(EntityType::HEART);
	heart->SetTexture(tex->Load("Assets/Textures/Entities/Items/heart.png"));
	heart->SetPlayer(player);
	heart->position = iPoint(6*64, 23*64);
	//checkpoint load
	checkpoint1 = (Checkpoint*)entityManager->CreateEntity(EntityType::CHECKPOINT);
	//checkpoint1 = SetTexture
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
	// Collision detection: map vs player
	tempPlayerPosition = player->position;
	player->Update(input,dt);
	CollisionHandler();

	//Debug Keys
	if (input->GetKey(SDL_SCANCODE_F9) == KeyState::KEY_DOWN)
	{
		map->drawColliders = !map->drawColliders;
		collisions->debug = !collisions->debug;
		
	}
	if (input->GetKey(SDL_SCANCODE_F3) == KeyState::KEY_DOWN)
	{
		TransitionToScene(SceneType::GAMEPLAY);
	}
	// L02: DONE 3: Request Load / Save when pressing L/S
	//if (input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) app->LoadGameRequest();
	//if (input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) app->SaveGameRequest();

	return true;
}

bool SceneGameplay::Draw(Render* render)
{

	render->CameraUpdate(player->position);
	DrawBackground(render);
	//Draw map
	map->Draw(render);
	
    return false;
}

bool SceneGameplay::Unload()
{
	// TODO: Unload all resources

    return false;
}

void SceneGameplay::CollisionHandler() 
{
	// Check if updated player position collides with next tile
	// IMPROVEMENT: Just check adyacent tiles to player
	for (int y = 0; y < map->data.height; y++)
	{
		for (int x = 0; x < map->data.width; x++)
		{
			if ((map->data.layers[4]->Get(x, y) >= 86) &&
				CheckCollision(map->GetTilemapRec(x, y), player->GetBounds()))
			{
				player->position = tempPlayerPosition;
				player->velocity.y = 0.0f;
				player->readyToJump = true;
				break;
			}
		}
	}
}
void SceneGameplay::DrawBackground(Render* render) {
	render->scale = 1.5;
	render->DrawTexture(background1, 0, 150, &backgroundRect, 0.2f);
	
	render->DrawTexture(background2, 0, 150, &backgroundRect, 0.4f);

	render->DrawTexture(background3, 0, 150, &backgroundRect, 0.6f);

	render->DrawTexture(background4, 0, 150, &backgroundRect, 0.8f);
	render->scale = 1;
}