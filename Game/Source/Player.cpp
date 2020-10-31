#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
#include "Collisions.h"
#include "Window.h"
//#include "StartScreen.h"
//#include "LoseScreen.h"
#include "FadeToBlack.h"
#include "Audio.h"
#include <math.h>

#include <stdio.h>

Player::Player() : Module() {
	name = "Player";
	
}

Player::~Player() {}

bool Player::Start() {
	bool ret = true;
	texture = app->tex->Load("Assets/player/PlayerSheet.png");
	//physics variables
	isLeft = false;
	
	initialPosition = { 50, app->win->GetWindowHeight() / 2 };
	position = initialPosition;
	gravity = 0.5f;
	velocityY = 0.0f;
	onGround = false;
	//Loading Idle Anim
	for (int i = 0; i <= (116*5); i+=116)
	{
		playerIdle.PushBack({ i,30,57,86 });
	}
	playerIdle.loop = true;
	playerIdle.speed = 0.09f;
	//Loading Running Anim
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
	//Loading Jumping Anim
	playerJumping.PushBack({ 0,262,75,86 });
	playerJumping.PushBack({ 116,261,70,87 });
	playerJumping.loop = true;
	playerJumping.speed = 0.06f;
	return ret;
}

bool Player::Update(float dt) {
	bool ret = true;
	velocityY += gravity;
	position.y += velocityY;
	if (position.y >= initialPosition.y)
	{
		position.y = initialPosition.y;
		velocityY = 0.0;
		onGround = true;
	}
	
	
	
		if (onGround) {
			if (current_anim != &playerIdle) {
				current_anim = &playerIdle;
			}
		}
		else {
			if (current_anim != &playerJumping) {
				current_anim = &playerJumping;
				playerJumping.Reset();
			}
		}
		
	
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		isLeft = true;
		if (onGround) {
			if (current_anim != &playerRunning) {
				current_anim = &playerRunning;
			}
		}
		else {
			if (current_anim != &playerJumping) {
				current_anim = &playerJumping;
				playerJumping.Reset();
			}
		}
		
		position.x -= 5;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		isLeft = false;
		if (onGround) {
			if (current_anim != &playerRunning) {
				current_anim = &playerRunning;
			}
		}
		else {
			if (current_anim != &playerJumping) {
				current_anim = &playerJumping;
				playerJumping.Reset();
			}
		}
		position.x += 5;
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
		
		Jump();
		
		
	}else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP) {
		Fall();
	}
	
	
	rectAnim = current_anim->GetCurrentFrame();
	if (!app->render->DrawTexture(texture, position.x, position.y, &rectAnim,isLeft)) {
		ret = false;
	}
	return ret;
}

bool Player::PostUpdate() {
	bool ret = true;
	
	return ret;
}

bool Player::CleanUp() {
	bool ret = true;

	

	return ret;
}

void Player::OnCollision(Collider* c1, Collider* c2) {
	// Detect collision with a bullet or an enemy. If so, disappear and explode.
	
}
void Player::Jump()
{
	if (onGround) {
		velocityY = -15.0;
		onGround = false;
	}
	
}
// Add acceleration to Y speed
void Player::Fall()
{
	
	if (velocityY < -9.0)
		velocityY = -9.0;
}