#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
#include "Collisions.h"
#include "Window.h"
#include "FadeToBlack.h"
#include "Audio.h"
#include <math.h>

#include <stdio.h>

Player::Player(bool startEnabled) : Module(startEnabled) {
	name = "Player";

}

Player::~Player() {}

bool Player::Start() {
	bool ret = true;
	texture = app->tex->Load("Assets/player/PlayerSheet.png");
	//physics variables
	isLeft = false;
	initialPosition = { 200, 900 };
	position = initialPosition;
	app->render->camera.x = position.x;
	app->render->camera.y = position.y;
	gravity = 0.5f;
	velocityY = 0.0f;
	onGround = false;
	//Loading Idle Anim
	for (int i = 0; i <= (116 * 5); i += 116)
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
	playerJumping.speed = 0.09f;
	//Loading Falling Anim
	playerFalling.PushBack({0,364,59,100});
	playerFalling.PushBack({116,364,59,100});
	playerFalling.loop = true;
	playerFalling.speed = 0.09f;
	//Loading Death Anim
	playerDeath.PushBack({ 0,603,65,93 });
	playerDeath.PushBack({ 116,600,73,96 });
	playerDeath.PushBack({ 232,597,73,96 });
	playerDeath.PushBack({ 348,595,102,101 });
	playerDeath.PushBack({ 464,595,105,101 });
	playerDeath.PushBack({ 580,595,101,101 });
	playerDeath.PushBack({ 696,595,105,101 });
	playerDeath.loop = false;
	playerDeath.speed = 0.12f;
	playerDeath.speed = 0.12f;
	return ret;
}

bool Player::Update(float dt) {
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


	if (onGround && !dead) {
		if (current_anim != &playerIdle) {
			current_anim = &playerIdle;
			playerJumping.Reset();
		}
	}else if(!dead){
		if (current_anim != &playerJumping) {
			current_anim = &playerJumping;
			playerJumping.Reset();
		}
	}
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
	else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP || app->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
	{
		Fall();
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



	return ret;
}

void Player::OnCollision(Collider* c1, Collider* c2)
{
	// Detect collision with a bullet or an enemy. If so, disappear and explode.

}

void Player::Run() {
	if (current_anim != &playerRunning && onGround)
	{
		current_anim = &playerRunning;
		playerJumping.Reset();
	}
	isLeft? position.x -= 5 : position.x += 5;
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
		playerJumping.Reset();
	}
}
bool Player::Died() {
	return (dead && playerDeath.Finished())? true : false;
}
void Player::UpdateCamera() {
	int xAxis = (-position.x) + (app->win->GetWindowWidth() / 2);
	int yAxis = (-position.y) + (app->win->GetWindowHeight() / 2);
	app->render->camera.x = xAxis;
	app->render->camera.y = yAxis;
	if (app->render->camera.x >= 0) {
		app->render->camera.x = 0;
	}
	else if (app->render->camera.x <= -1275) {
		app->render->camera.x = -1275;
	}
}