#pragma once
#include "Entity.h"

class Checkpoint : public Entity
{
public:
	Checkpoint();
	~Checkpoint();

	bool Update(float dt) override;
	void OnCollision(Collider* collider) override;

public:

	bool collision;
	int fx;

	bool passedCheckpoint;

	Animation redAnim;
	Animation blueAnim;
};