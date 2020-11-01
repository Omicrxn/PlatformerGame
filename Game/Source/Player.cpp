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

	playerIdle.PushBack({ 25,23,17,25 });
	playerIdle.PushBack({ 89,23,17,25 });
	playerIdle.PushBack({ 153,23,17,25 });
	playerIdle.PushBack({ 217,23,17,25 });
	playerIdle.PushBack({ 217,23,17,25 });
	playerIdle.PushBack({ 281,23,17,25 });
	playerIdle.loop = true;
	playerIdle.speed = 0.09f;

	// Loading Running Anim
	playerRunning.PushBack({ 26,86,17,25 });
	playerRunning.PushBack({ 90,86,17,25 });
	playerRunning.PushBack({ 154,86,17,26 });
	playerRunning.PushBack({ 217,86,17,26 });
	playerRunning.PushBack({ 280,86,19,25 });
	playerRunning.PushBack({ 343,86,21,25 });
	playerRunning.PushBack({ 408,86,19,26 });
	playerRunning.PushBack({ 473,86,19,26 });

	playerRunning.loop = true;
	playerRunning.speed = 0.12f;

	// Loading Jumping Anim
	playerJumping.PushBack({ 26,151,17,25 });
	playerJumping.PushBack({ 89,151,17,25 });
	playerJumping.PushBack({ 152,151,17,26 });

	playerJumping.loop = true;
	playerJumping.speed = 0.09f;

	// Loading Death Anim
	playerDeath.PushBack({ 7,474,36,22 });
	playerDeath.PushBack({ 80,474,36,22 });
	playerDeath.PushBack({ 142,474,41,22 });
	playerDeath.PushBack({ 209,474,41,22 });
	playerDeath.PushBack({ 272,474,41,22 });

	playerDeath.loop = false;
	playerDeath.speed = 0.12f;
}

Player::~Player() {}

bool Player::Start() 
{
	bool ret = true;

	texture = app->tex->Load("Assets/player/Player.png");
	
	// Physics variables
	isLeft = false;
	initialPosition = { 200, 900};
	position = initialPosition;
	app->render->camera.x = (-initialPosition.x) + (app->win->GetWindowWidth() / 2);
	app->render->camera.y = (-position.y) + (app->win->GetWindowHeight() / 2);
	gravity = 1;
	velocity = {0,0};
	onGround = true;
	dead = false;
	collision = false;
	scale = app->win->GetScale();
	return ret;
}

bool Player::Update(float dt) 
{
	bool ret = true;
	iPoint tempPlayerPosition = position;
	if (!dead) {
		
		velocity.y += gravity;
		position.y += velocity.y;
		isLeft ? position.x -= velocity.x : position.x += velocity.x;
	}
	
	if (collision == true)
	{
		if (onGround || dead) {
			velocity.y = 0.0;
			position.y = tempPlayerPosition.y;
			collision = false;
		}
	}else{
		Fall();
	}
	UpdateCamera();
	if (onGround && !dead) 
	{
		if (current_anim != &playerIdle) 
		{
			current_anim = &playerIdle;
			playerJumping.Reset();
		}
		velocity.x = 0;
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
	
	
	// Update player position
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !dead)
	{
		isLeft = true;
		Run();
	}
	
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !dead)
	{
		isLeft = false;
		Run();
	}

	
	rectAnim = current_anim->GetCurrentFrame();
	// Map collisions detection (platforms)
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
					//Red Collider
					if (layer->data->Get(x, y) == 4097 && CheckCollision(tileRect, playerRect))
					{
						collision = true;
						if (playerRect.y < tileRect.y) {
							onGround = true;
						}
						else if (playerRect.y > tileRect.y) {
							velocity.y = 1;
						}
						
						break;
					}
					//Green Collider
					if (layer->data->Get(x, y) == 4098 && CheckCollision(tileRect, playerRect))
					{
						app->fade->Fade((Module*)app->scene, (Module*)app->titleScreen, 180);
					}
					//Blue Collider
					if (layer->data->Get(x, y) == 4099 && CheckCollision(tileRect, playerRect)) {
						collision = true;
						velocity.x *= -1;
						if (isLeft)
						{
							position.x += 3;
						}
						else
						{
							position.x -= 3;
						}
						break;
					}
					//Yellow Collider
					if (layer->data->Get(x, y) == 4100 && CheckCollision(tileRect, playerRect))
					{
						collision = true;
						app->player->Die();
						break;
					}
				}
			}
		}

		layer = layer->next;
	}

	if ((app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) && !dead)
	{
		Jump();
	}
	else if ((app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP || app->input->GetKey(SDL_SCANCODE_W) == KEY_UP) && !dead)
	{
		SmallJump();
	}

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
	velocity.x = 4;
}

void Player::Jump()
{
	if (onGround)
	{
		velocity.y = -15.0f;
		onGround = false;
	}
}

// Add acceleration to Y speed
void Player::SmallJump()
{
	if (velocity.y < -9.0f)
		velocity.y = -9.0f;
}

void Player::Fall() 
{
	if (velocity.y > 0)
		velocity.y = 8;
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
	int xAxis = (-position.x * scale) + (app->win->GetWindowWidth() / 2 );
	int yAxis = (-position.y*scale) + ((app->win->GetWindowHeight() / 2) );
	app->render->camera.x = xAxis ;
	app->render->camera.y = yAxis ;

	if (app->render->camera.x >= 0)
	{
		app->render->camera.x = 0;
	}
	else if (app->render->camera.x <= -1275)
	{
		app->render->camera.x = -1275;
	}

	if (app->render->camera.y >= -6)
	{
		app->render->camera.y = -6;
	}
	else if (app->render->camera.y <= -715)
	{
		app->render->camera.y = -715;
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