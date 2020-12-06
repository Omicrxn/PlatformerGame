#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL_rect.h"

struct SDL_Texture;

class Player : public Entity 
{
public:

	Player();
	
	~Player();
	
	bool Update(float dt) override;
	void OnCollision(Collider* collider) override;
	
	void Run(bool isLeft);
	void Jump();
	void SmallJump();

	void Fall();

	void Die();
	bool Died();
	void GroundCollisions();
	
	void UpdateCamera();

	bool CheckCollision(SDL_Rect tileRect, SDL_Rect playerRect);
	void PrintData();

	bool godMode = false;

private:
	// Physics variables

	bool onGround;

	bool collision;

	uint scale;

	// Idle Animation
	Animation playerIdle;

	// Running Animation
	Animation playerRunning;

	// Jumping Animation
	Animation playerJumping;

	// Falling Animation
	Animation playerFalling;

	// Death Animation
	Animation playerDeath;

public:
	int score;
	int lifes;

	bool hasCheckpoint;
	iPoint currentCheckpoint;

	int fx;
};

#endif // __PLAYER_H__