#pragma once
#include "Entity.h"
class Heart : public Entity
{
public:
	Heart();
	~Heart();

	bool Update(float dt) override;
	void OnCollision(Collider* collider) override;
public:

	bool collision;
	int fx;
	Animation movingAnim;

};