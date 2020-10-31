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
	airborne = false;
	grounded = true;
	initialPosition = { 50, app->win->GetWindowHeight() / 2 };
	position = initialPosition;
	gravity = 50.0f;
	speedY = 7;
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
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE ) {
		if (current_anim != &playerIdle) {
			current_anim = &playerIdle;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		isLeft = true;
		if (current_anim != &playerRunning) {
			current_anim = &playerRunning;
		}
		position.x -= 5;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		isLeft = false;
		if (current_anim != &playerRunning) {
			current_anim = &playerRunning;
		}
		position.x += 5;
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		if (current_anim != &playerJumping) {
			current_anim = &playerJumping;
			playerJumping.Reset();
		}
		if (position.y <= 200) {
			airborne = false;
			
		}else{
			airborne = true;
		}
		
		
	}else if (airborne && (app->input->GetKey(SDL_SCANCODE_W) == KEY_UP || app->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE)) {
			airborne = false;
	}
	
	if (airborne && position.y > 200) {
		grounded = false;
		if(!grounded)Jump();
	}else if(!airborne && position.y < initialPosition.y){
		Fall((1.0f / 60.0f));
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

	position.y = position.y - speedY ;
}
// Add acceleration to Y speed
void Player::Fall(float dt)
{
	
	position.y = position.y + speedY ;
}