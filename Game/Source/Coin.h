#pragma once
#include "Entity.h"
class Coin : public Entity
{
public:
	Coin();
	~Coin();

	bool Update(float dt) override;

public:

	bool collision;

	Animation movingAnim;

};