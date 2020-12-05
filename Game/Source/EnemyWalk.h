#pragma once
#include "Entity.h"
class EnemyWalk : public Entity
{
public:
	EnemyWalk();
	~EnemyWalk();

	bool Update(float dt) override;
private:
	void Move();

public:

	bool collision;

	Animation movingAnim;
	
};