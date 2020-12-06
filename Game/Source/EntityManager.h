#pragma once
#include "Entity.h"
#include "List.h"
#include "Module.h"

class EntityManager : public Module
{
public:
	EntityManager(bool startEnable);
	
	~EntityManager();

	Entity* CreateEntity(EntityType type);
	void DestroyEntity(Entity* entity);
	
	bool Update(float dt);
	bool UpdateAll(float dt, bool doLogic);
	bool CleanUp();

	// Called when an enemi collider hits another collider
	// The enemy is destroyed and an explosion particle is fired
	void OnCollision(Collider* c1, Collider* c2);

	List<Entity*> entities;

	bool doLogic = false;

	float accumulatedTime = 0;
};