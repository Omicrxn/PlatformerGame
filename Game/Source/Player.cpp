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


#include <stdio.h>

Player::Player() : Module() {
	name = "Player";
}

Player::~Player() {}

bool Player::Start() {
	bool ret = true;

	destroyed = false;

	currentFuel = maxFuel;

	// Loading the image into a texture
	texture = app->tex->Load("Assets/sprites/playablecharacters/player spaceships.png");


	// Position of the spaceship in the screen
	position.x = 10;
	position.y = app->win->GetWindowHeight() / 2;

	// Position of the rectangle that we are getting from the image we load
	playerAnimRight.PushBack({ 155,187,32,9 });
	playerAnimRight.PushBack({ 190,186,32,12 });
	playerAnimRight.PushBack({ 120,185,32,16 });

	playerAnimLeft.PushBack({ 48,187,32,9 });
	playerAnimLeft.PushBack({ 13,186,32,12 });
	playerAnimLeft.PushBack({ 83,185,32,16 });

	playerSpinningRight.PushBack({ 237,175,36,20 });
	playerSpinningRight.PushBack({ 289,174,28,22 });
	playerSpinningRight.PushBack({ 340,172,20,26 });
	playerSpinningRight.PushBack({ 391,170,18,30 });
	playerSpinningRight.PushBack({ 427,172,17,25 });
	playerSpinningRight.PushBack({ 469,174,27,21 });

	playerSpinningLeft.PushBack({ 242,216,28,23 });
	playerSpinningLeft.PushBack({ 285,215,19,25 });
	playerSpinningLeft.PushBack({ 333,213,15,29 });
	playerSpinningLeft.PushBack({ 377,216,19,24 });
	playerSpinningLeft.PushBack({ 422,216,27,23 });
	playerSpinningLeft.PushBack({ 467,218,35,20 });

	// Loading shooting sound effect
	shootFx = app->audio->LoadFx("Assets/music/events/shoot1.wav");

	// Loading collision sound effect
	hitFx = app->audio->LoadFx("Assets/music/events/collisionswithobjects.wav");


	// Loading collision sound effect
	dieFx = app->audio->LoadFx("Assets/music/events/die.wav");


	// Add a collider to the player
	collider = app->collisions->AddCollider({ position.x, position.y, 32, 16 }, Collider::Type::PLAYER, this);

	return ret;
}

bool Player::Init() {
	bool ret = true;

	return ret;
}

bool Player::Update(float dt) {
	bool ret = true;


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