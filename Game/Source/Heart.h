#pragma once
#include "Entity.h"
class Heart : public Entity
{
public:
	Heart();
	~Heart();

	bool Update(float dt) override;

public:

	bool collision;

	Animation movingAnim;

};