#pragma once
#include "Entity.h"
class EnemyFly : public Entity
{
public:
	EnemyFly();
	~EnemyFly();

	bool Update(float dt) override;
private:
	void Move();

public:

	bool collision;

	Animation movingAnim;
	
};