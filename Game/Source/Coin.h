#pragma once
#include "Entity.h"
class Coin : public Entity
{
public:
	Coin();
	~Coin();

	bool Update(float dt) override;
	void OnCollision(Collider* collider) override;
public:

	bool collision;
	int fx;
	Animation movingAnim;

};