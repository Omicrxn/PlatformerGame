#pragma once
#include "Entity.h"
class EnemyFly : public Entity
{
public:
	EnemyFly();
	~EnemyFly();

	bool Update(float dt) override;

	void UpdatePath();

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