#include "EntityManager.h"
#include "Collisions.h"

#include "Player.h"
#include "Enemy.h"
#include "Checkpoint.h"
#include "Coin.h"
#include "Heart.h"
#include "Map.h"
#include "Collisions.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager(Render* render,Collisions* collisions) : Module()
{
	name.Create("entitymanager");
	this->collisions = collisions;
	this->render = render;
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	if (!active) return true;

	return true;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* ret = nullptr;

	switch (type)
	{
		// L13: Create the corresponding type entity
		case EntityType::PLAYER: ret = new Player(collisions,this);  break;
		//case EntityType::ENEMY: ret = new Enemy();  break;
		case EntityType::COIN: ret = new Coin(collisions,this);break;
		case EntityType::HEART: ret = new Heart(collisions,this);break;
		case EntityType::CHECKPOINT: ret = new Checkpoint(collisions,this);break;
		default: break;
	}

	// Created entities are added to the list
	if (ret != nullptr) entities.Add(ret);

	return ret;
}

bool EntityManager::Update(float dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

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
	ListItem<Entity*>* entity = entities.start;
	while (entity != nullptr)
	{
		
		if (doLogic) entity->data->Update(dt);
		entity->data->Draw(render);
		if (!entity->data->active)DestroyEntity(entity->data);
		entity = entity->next;
	}

	return true;
}
void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item = entities.At(entities.Find(entity));
	//app->tex->Unload(entity->texture);
	//entity->texture = nullptr;
	RELEASE(entity);

	entities.Del(item);
}
void EntityManager::OnCollision(Collider* c1, Collider* c2)
{
	for (ListItem<Entity*>* i = entities.start; i != NULL; i = i->next)
	{
		if (i->data->GetCollider() == c1 && ((c2->type == Collider::Type::PLAYER_BULLET) || (c2->type == Collider::Type::PLAYER) || (c2->type == Collider::Type::ENEMY)))
		{
			i->data->OnCollision(c2);
		}
	}
}
