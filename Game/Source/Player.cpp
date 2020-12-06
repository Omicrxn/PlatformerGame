#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"
#include "FadeToBlack.h"
#include "Audio.h"
#include "Map.h"
#include "Scene.h"
#include "Particles.h"
#include "EntityManager.h"

#include <stdio.h>
#include <math.h>

Player::Player() : Entity(EntityType::PLAYER) 
{
	// Loading Idle Anim
	playerIdle.PushBack({ 25,23,17,25 });
	playerIdle.PushBack({ 89,23,17,25 });
	playerIdle.PushBack({ 153,23,17,25 });
	playerIdle.PushBack({ 217,23,17,25 });
	playerIdle.PushBack({ 217,23,17,25 });
	playerIdle.PushBack({ 281,23,17,25 });

	playerIdle.loop = true;
	playerIdle.speed = 0.1f;

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

	texture = app->tex->Load("Assets/Player/player.png");
	fx = app->audio->LoadFx("Assets/Audio/Fx/shoot.wav");

	// Physics variables
	isLeft = false;

	initialPosition = { 200,800 };
	position = initialPosition;

	app->render->camera.x = (-initialPosition.x) + (app->win->GetWindowWidth() / 2);
	app->render->camera.y = (-position.y) + (app->win->GetWindowHeight() / 2);

	gravity = 600.0f;

	velocity = { 0,0 };

	score = 0;
	lifes = 3;
	hasCheckpoint = false;

	onGround = true;
	dead = false;
	collision = false;

	collider = app->collisions->AddCollider({ position.x,position.y,17,25 }, Collider::Type::PLAYER, (Module*)app->entityman);

	scale = app->win->GetScale();
	PrintData();
}

Player::~Player() {}

bool Player::Update(float dt)
{
	bool ret = true;

	iPoint tempPlayerPosition = position;
	if (!dead && !godMode)
	{
		position.x = position.x + velocity.x * dt;
		position.y = position.y + velocity.y * dt + (gravity * dt * dt * 0.5);
		velocity.y = velocity.y + gravity * dt;
	}

	if (collision == true)
	{
		if (onGround || dead) 
		{
			velocity.y = 0.0;
			position.y = tempPlayerPosition.y;
			collision = false;
		}
	}
	else
	{
		Fall();
	}

	UpdateCamera();

	if (onGround && !dead)
	{
		if (current_anim != &playerIdle)
		{
			current_anim = &playerIdle;
			playerIdle.Reset();
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

	// Shoot
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->audio->PlayFx(fx);

		if (isLeft && app->particles->playerBullet.speed.x > 0)
		{
			app->particles->playerBullet.speed.x = -app->particles->playerBullet.speed.x;
		}
		else if (!isLeft && app->particles->playerBullet.speed.x < 0) 
		{
			app->particles->playerBullet.speed.x = -app->particles->playerBullet.speed.x;
		}

		app->particles->AddParticle(app->particles->playerBullet, position.x + 32, position.y, Collider::Type::PLAYER_BULLET);
	}

	// F3 Start from the beginning of the current level
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		position.x = initialPosition.x;
		position.y = initialPosition.y;
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && !dead && godMode)
	{
		position.y += 3;
	}

	if (app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN && hasCheckpoint)
	{
		position.x = currentCheckpoint.x;
		position.y = currentCheckpoint.y - 5;
	}

	rectAnim = current_anim->GetCurrentFrame();

	GroundCollisions();

	// Update collider position
	if (collider != nullptr)
	{
		collider->SetPos(position.x, position.y);
	}
	if (!app->render->DrawTexture(texture, position.x, position.y, &rectAnim, isLeft))
	{
		ret = false;
	}

	return ret;
}

void Player::Run(bool isLeft)
{
	this->isLeft = isLeft;
	
	if (godMode)
	{
		isLeft ? position.x -= 3 : position.x += 3;
	}
	else
	{
		if (current_anim != &playerRunning && onGround)
		{
			current_anim = &playerRunning;
			playerRunning.Reset();
		}
		isLeft ? velocity.x = -200.0f : velocity.x = 250.0f;
	}
}

void Player::Jump()
{
	if (godMode)
	{
		position.y -= 3;
	}
	else
	{
		if (onGround)
		{
			velocity.y = -300.0f;
			onGround = false;
		}
	}
}

// Add acceleration to Y speed
void Player::SmallJump()
{
	if (velocity.y < -250.0f && !godMode) velocity.y = -250.0f;
}

void Player::Fall()
{
	if (velocity.y > 0) velocity.y = 400.0f;
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

void Player::GroundCollisions()
{
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

					// Red Collider
					if (layer->data->Get(x, y) == 4097 && CheckCollision(tileRect, playerRect) && !godMode)
					{
						collision = true;
						if (playerRect.y < tileRect.y)
						{
							onGround = true;
						}
						else if (playerRect.y > tileRect.y)
						{
							velocity.y = 100.0f;
						}

						break;
					}

					// Green Collider
					if (layer->data->Get(x, y) == 4098 && CheckCollision(tileRect, playerRect) && !godMode)
					{
						app->fade->Fade((Module*)app->scene, (Module*)app->titleScreen, 180);
					}

					// Blue Collider
					if (layer->data->Get(x, y) == 4099 && CheckCollision(tileRect, playerRect) && !godMode)
					{
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

					// Yellow Collider
					if (layer->data->Get(x, y) == 4100 && CheckCollision(tileRect, playerRect))
					{
						collision = true;

						if (!godMode)
						{
							Die();
						}

						if (lifes > 0)
						{
							lifes--;
							PrintData();
						}
						else
						{
							if (hasCheckpoint)
							{
								position.x = currentCheckpoint.x;
								position.y = currentCheckpoint.y - 5;
							}
							else
							{
								Die();
							}
						}
						break;
					}
				}
			}
		}
		layer = layer->next;
	}
}

void Player::UpdateCamera()
{
	int xAxis = (-position.x * scale) + (app->win->GetWindowWidth() / 2);
	int yAxis = (-position.y * scale) + ((app->win->GetWindowHeight() / 2));

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

	if (app->render->camera.y >= -6)
	{
		app->render->camera.y = -6;
	}
	else if (app->render->camera.y <= -720)
	{
		app->render->camera.y = -720;
	}
}

void Player::PrintData()
{
	system("CLS");
	printf("Lifes: %d\n", lifes);
	printf("Score: %d\n", score);
}

bool Player::CheckCollision(SDL_Rect tileRect, SDL_Rect playerRect)
{
	// Check if there is an overlap between two rects
	return (tileRect.x < playerRect.x + playerRect.w &&
		tileRect.x + tileRect.w > playerRect.x &&
		tileRect.y < playerRect.y + playerRect.h &&
		tileRect.y + tileRect.h > playerRect.y);
}

void Player::OnCollision(Collider* collider)
{
	if (collider->type == Collider::Type::ENEMY)
	{
		dead = true;
		app->audio->PlayFx(fx);
		app->scene->player->lifes--;
		app->scene->player->PrintData();
		app->entityman->DestroyEntity(this);
		app->fade->Fade((Module*)app->scene, (Module*)app->endingScreen, 180);
	}
}