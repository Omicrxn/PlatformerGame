#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
#include "Collisions.h"
#include "Window.h"
#include "FadeToBlack.h"
#include "Audio.h"
#include "Map.h"

#include <stdio.h>
#include <math.h>

Player::Player(bool startEnabled) : Module(startEnabled) 
{
	name = "player";

	// Loading Idle Anim
	for (int i = 0; i <= (116 * 5); i += 116)
	{
		playerIdle.PushBack({ i,30,57,86 });
	}
	playerIdle.loop = true;
	playerIdle.speed = 0.09f;

	// Loading Running Anim
	playerRunning.PushBack({ 0,146,75,86 });
	playerRunning.PushBack({ 116,146,70,86 });
	playerRunning.PushBack({ 232,146,59,86 });
	playerRunning.PushBack({ 348,146,71,86 });
	playerRunning.PushBack({ 464,146,75,86 });
	playerRunning.PushBack({ 580,146,63,86 });
	playerRunning.PushBack({ 696,146,55,86 });
	playerRunning.PushBack({ 811,146,71,86 });
	playerRunning.loop = true;
	playerRunning.speed = 0.12f;

	// Loading Jumping Anim
	playerJumping.PushBack({ 0,262,75,86 });
	playerJumping.PushBack({ 116,261,70,87 });
	playerJumping.loop = true;
	playerJumping.speed = 0.09f;

	// Loading Falling Anim
	playerFalling.PushBack({ 0,364,59,100 });
	playerFalling.PushBack({ 116,364,59,100 });
	playerFalling.loop = true;
	playerFalling.speed = 0.09f;

	// Loading Death Anim
	playerDeath.PushBack({ 0,603,65,93 });
	playerDeath.PushBack({ 116,600,73,96 });
	playerDeath.PushBack({ 232,597,73,96 });
	playerDeath.PushBack({ 348,595,102,101 });
	playerDeath.PushBack({ 464,595,105,101 });
	playerDeath.PushBack({ 580,595,101,101 });
	playerDeath.PushBack({ 696,595,105,101 });
	playerDeath.loop = false;
	playerDeath.speed = 0.12f;
}

Player::~Player() {}

bool Player::Start() 
{
	bool ret = true;

	texture = app->tex->Load("Assets/player/PlayerSheet.png");
	
	// Physics variables
	isLeft = false;
	initialPosition = { 200, 900 };
	position = initialPosition;
	app->render->camera.x = (-initialPosition.x) + (app->win->GetWindowWidth() / 2);
	app->render->camera.y = (-position.y) + (app->win->GetWindowHeight() / 2);
	gravity = 0.5f;
	velocityY = 0.0f;
	onGround = false;
	dead = false;
	
	return ret;
}

bool Player::Update(float dt) 
{
	bool ret = true;

	velocityY += gravity;
	position.y += velocityY;
	UpdateCamera();
	
	if (position.y >= initialPosition.y)
	{
		position.y = initialPosition.y;
		velocityY = 0.0;
		onGround = true;
	}

	if (onGround && !dead) 
	{
		if (current_anim != &playerIdle) 
		{
			current_anim = &playerIdle;
			playerJumping.Reset();
		}
	}
	else if (!dead)
	{
		if (current_anim != &playerJumping) 
		{
			current_anim = &playerJumping;
			playerJumping.Reset();
		}
	}

	// F3 Start from the beginning of the current level
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		position.x = initialPosition.x;
		position.y = initialPosition.y;
	}

	// Saving the player position for collision cases
	iPoint tempPlayerPosition = position;
	
	// Update player position
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		isLeft = true;
		Run();
	}
	
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		isLeft = false;
		Run();
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		Jump();
	}
	/*else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP || app->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
	{
		Fall();
	}*/
	
	// Map collisions detection (platforms)
	bool collision = false;
	SDL_Rect playerRect = { position.x, position.y, rectAnim.w, rectAnim.h };
	iPoint tilePosition;
	SDL_Rect tileRect;

	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	while (layer != NULL)
	{
		if (layer->data->name == "Collisions")
		{
			for (int y = 0; y < app->map->data.height; y++)
			{
				for (int x = 0; x < app->map->data.width; x++)
				{
					tilePosition = app->map->MapToWorld(x, y);
					tileRect = { tilePosition.x, tilePosition.y, app->map->data.tileWidth, app->map->data.tileHeight };
					if (layer->data->Get(x, y) == 4097 && CheckCollision(tileRect, playerRect))
					{
						collision = true;
						break;
					}
				}
			}
		}

		layer = layer->next;
	}

	if (collision == true)
	{
		position = tempPlayerPosition;
	}

	rectAnim = current_anim->GetCurrentFrame();
	if (!app->render->DrawTexture(texture, position.x, position.y, &rectAnim, isLeft))
	{
		ret = false;
	}

	return ret;
}

bool Player::PostUpdate()
{
	bool ret = true;
	
	return ret;
}

bool Player::CleanUp()
{
	bool ret = true;
	app->tex->UnLoad(texture);
	current_anim = nullptr;

	return ret;
}

bool Player::LoadState(pugi::xml_node& data)
{
	position.x = data.child("player").attribute("x").as_int();
	position.y = data.child("player").attribute("y").as_int();

	return true;
}

bool Player::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node player = data.append_child("player");

	player.append_attribute("x") = position.x;
	player.append_attribute("y") = position.y;

	return true;
}

void Player::OnCollision(Collider* c1, Collider* c2)
{
	// Detect collision with a bullet or an enemy. If so, disappear and explode.

}

void Player::Run()
{
	if (current_anim != &playerRunning && onGround)
	{
		current_anim = &playerRunning;
		playerJumping.Reset();
	}
	isLeft ? position.x -= 5 : position.x += 5;
}

void Player::Jump()
{
	if (onGround)
	{
		velocityY = -15.0f;
		onGround = false;
	}
}

// Add acceleration to Y speed
void Player::Fall()
{
	if (velocityY < -9.0f)
		velocityY = -9.0f;
}

void Player::Die()
{
	dead = true;
	if (current_anim != &playerDeath && onGround)
	{
		current_anim = &playerDeath;
		playerDeath.Reset();
	}
}

bool Player::Died()
{
	return (dead && playerDeath.Finished()) ? true : false;
}

void Player::UpdateCamera()
{
	int xAxis = (-position.x) + (app->win->GetWindowWidth() / 2);
	int yAxis = (-position.y) + (app->win->GetWindowHeight() / 2);
	app->render->camera.x = xAxis;
	app->render->camera.y = yAxis;

	if (app->render->camera.x >= 0)
	{
		app->render->camera.x = 0;
	}
	else if (app->render->camera.x <= -1275)
	{
		app->render->camera.x = -1275;
	}
}

bool Player::CheckCollision(SDL_Rect tileRect, SDL_Rect playerRect)
{
	// Check if there is an overlap between two rects
	return (tileRect.x < playerRect.x + playerRect.w &&
		tileRect.x + tileRect.w > playerRect.x &&
		tileRect.y < playerRect.y + playerRect.h &&
		tileRect.y + tileRect.h > playerRect.y);
}