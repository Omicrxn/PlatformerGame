#pragma once
#ifndef __MODULEPLAYER_H__
#define __MODULEPLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL_rect.h"
#include "Animation.h"

struct SDL_Texture;

class Player : public Entity 
{
public:

	Player();
	
	~Player();
	
	
	bool Update(float dt);
	
	/*bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;*/
	
	void Run();
	void Jump();
	void SmallJump();

	void Fall();

	void Die();
	bool Died();
	
	void UpdateCamera();

	// Get the current player position
	iPoint GetPlayerPosition() 
	{
		return position;
	}

	bool CheckCollision(SDL_Rect tileRect, SDL_Rect playerRect);

private:

	// Physics variables

	iPoint initialPosition;

	iPoint velocity;
	float gravity;

	bool collision;
	bool onGround;

	bool dead;

	uint scale;


	Animation* current_anim = nullptr;

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


	bool godMode = false;
	bool isLeft;
};

#endif // __MODULEPLAYER_H__