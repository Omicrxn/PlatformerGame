#pragma once
#include "Point.h"
#include "SDL/include/SDL_rect.h"
#include "Animation.h"
#include "DynArray.h"
#include "Collisions.h"
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
	~Entity()
	{
		if (collider != nullptr)
			collider->pendingToDelete = true;
	}
	virtual bool Update(float dt) { return true; }
	// Returns the enemy's collider
	const Collider* GetCollider() const {return collider;}
	virtual void OnCollision(Collider* collider) {};
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
	// The enemy's collider
	Collider* collider = nullptr;
	bool dead;
	bool isLeft;
	Animation* current_anim = nullptr;
	SDL_Texture* texture;
	SDL_Rect rectAnim;
};