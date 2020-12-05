#pragma once
#include "Point.h"
#include "SDL/include/SDL_rect.h"

struct SDL_Texture;
enum class EntityType
{
	PLAYER,
	ENEMY_WALK,
	ENEMY_FLY,
	ITEM_COIN,
	UNKNOWN
};

class Entity
{

	
public:
	Entity(EntityType type) : type(type) {}
	virtual bool Update(float dt) { return true; }
	EntityType type;
	iPoint position;
	SDL_Texture* texture;
	SDL_Rect rectAnim;
};