#pragma once
#include "Entity.h"
class EnemyWalk : public Entity
{
public:
	EnemyWalk();
	~EnemyWalk();

	bool Update(float dt) override;
	void OnCollision(Collider* collider) override;
private:
	void Move();

public:

	bool collision;

	Animation movingAnim;
	
	// Variables to manage the enemies path
	DynArray<iPoint> path;
	iPoint origin;
	iPoint goal;
	float counter;
};