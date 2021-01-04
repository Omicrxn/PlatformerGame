#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"

#include "List.h"
class Collisions;
class Render;
class AudioManager;

class EntityManager : public Module
{
public:

	EntityManager(Render* render, Collisions* collisions, AudioManager* audio);

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type);
	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);

	bool UpdateAll(float dt, bool doLogic);

	void OnCollision(Collider* c1, Collider* c2);
public:

	List<Entity*> entities;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;
private:
	Collisions* collisions;
	Render* render;
	AudioManager* audio;
};

#endif // __ENTITYMANAGER_H__