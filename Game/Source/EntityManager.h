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
	List<Entity*> entities;
	bool doLogic = false;
	float accumulatedTime = 0;
};