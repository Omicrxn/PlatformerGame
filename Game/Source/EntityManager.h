#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"

#include "List.h"
class Collisions;
class Render;
class AudioManager;
class Particles;

class EntityManager : public Module
{
public:

	EntityManager(Render* render, Collisions* collisions, AudioManager* audio, Particles* particles );

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake();

	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type);
	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);

	bool UpdateAll(float dt, bool doLogic);

	void OnCollision(Collider* c1, Collider* c2);

	// Load / Save
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

public:

	List<Entity*> entities;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;

	bool doLogic = false;

private:

	Collisions* collisions;
	Render* render;
	AudioManager* audio;
	Particles* particles;
};

#endif // __ENTITYMANAGER_H__