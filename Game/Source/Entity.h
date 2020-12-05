#pragma once
#include "Point.h"
#include "SDL/include/SDL_rect.h"
#include "Animation.h"
#include "DynArray.h"

struct SDL_Texture;
enum class EntityType
{
	PLAYER,
	ENEMY_WALK,
	ENEMY_FLY,
	ITEM_COIN,
	ITEM_HEART,
	UNKNOWN
};

class Entity
{
public:
	Entity(EntityType type) : type(type) {}
	virtual bool Update(float dt) { return true; }
	// Get the current player position
	iPoint GetPlayerPosition()
	{
		return position;
	}

public:
	EntityType type;
	iPoint initialPosition;
	iPoint position;
	iPoint velocity;
	float gravity;

	bool dead;
	bool isLeft;
	Animation* current_anim = nullptr;
	SDL_Texture* texture;
	SDL_Rect rectAnim;
};