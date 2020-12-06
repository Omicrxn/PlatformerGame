#include "EntityManager.h"
#include "App.h"
#include "Player.h"
#include "EnemyFly.h"
#include "EnemyWalk.h"
#include "Coin.h"
#include "Heart.h"
#include "Textures.h"


EntityManager::EntityManager(bool startEnable) : Module(startEnable) 
{
	name = "entitymanager";

}

Entity* EntityManager::CreateEntity(EntityType type) 
{
	Entity* ret = nullptr;
	switch (type)
	{
	case EntityType::PLAYER:
		ret = new Player();
		break;
	case EntityType::ENEMY_WALK:
		ret = new EnemyWalk();
		break;
	case EntityType::ENEMY_FLY:
		ret = new EnemyFly();
		break;
	case EntityType::ITEM_COIN:
		ret = new Coin();
		break;
	case EntityType::ITEM_HEART:
		ret = new Heart();
		break;
	case EntityType::UNKNOWN:
		break;
	default:
		break;
	}
	if (ret != nullptr) entities.Add(ret);
	return ret;
}

bool EntityManager::Update(float dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= 0) doLogic = true;
	UpdateAll(dt, doLogic);

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}
	return true;
}
bool EntityManager::UpdateAll(float dt, bool doLogic)
{
	bool ret = true;
	ListItem<Entity*>* entity = entities.start;
	while (entity != nullptr) {
		if (doLogic)
		{
			entity->data->Update(dt);
		}
		entity = entity->next;
	}
	
	return ret;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	app->tex->Unload(entity->texture);
	entity->texture = nullptr;
	delete entity;
}

bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* entity = entities.end;
	while (entity != nullptr) {
		DestroyEntity(entity->data);
		entity = entity->prev;
	}
	entities.Clear();

	return ret;
}