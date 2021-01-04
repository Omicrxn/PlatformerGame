#pragma once
#ifndef __HEART_H__
#define __HEART_H__

#include "Entity.h"
#include "Render.h"
#include "Collisions.h"
#include "EntityManager.h"

#include "Point.h"
#include "SString.h"
class Player;
class Heart : public Entity
{
public:
	Heart(Collisions* collisions, EntityManager* entityManager);
	~Heart();

	void SetTexture(SDL_Texture* tex);
	bool Update(float dt) override;
	void Draw(Render* render);
	void SetPlayer(Player* player);
	void OnCollision(Collider* collider) override;
public:
	SDL_Texture* texture;
	Animation heartAnimation;

	int fx;
	Player* player;
};
#endif //__HEART_H__