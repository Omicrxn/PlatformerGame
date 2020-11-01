#pragma once
#ifndef __MODULEPLAYER_H__
#define __MODULEPLAYER_H__

#include "Module.h"
#include "Point.h"
#include "SDL/include/SDL_rect.h"
#include "Animation.h"

struct SDL_Texture;

class Player : public Module 
{
public:
	Player(bool startEnabled);
	
	~Player();
	
	bool Start();
	
	bool Update(float dt);
	
	bool PostUpdate();
	
	bool CleanUp();

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;
	
	void Run();
	void Jump();
	void Fall();
	
	void Die();
	bool Died();
	
	void UpdateCamera();
	
	// The player's collider
	Collider* collider = nullptr;

	// Collision callback, called when the player intersects with another collider
	void OnCollision(Collider* c1, Collider* c2);

	iPoint GetPlayerPosition() 
	{
		return position;
	}

private:
	// Physics variables
	iPoint position;
	iPoint initialPosition;

	float velocityY;
	float gravity;

	bool onGround;
	bool dead;

	SDL_Texture* texture = nullptr;

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

	SDL_Rect rectAnim;

	bool godMode = false;
	bool isLeft;
};

#endif // __MODULEPLAYER_H__