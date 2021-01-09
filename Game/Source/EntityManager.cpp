#include "EntityManager.h"
#include "Collisions.h"

#include "Player.h"
#include "EnemyFly.h"
#include "EnemyWalk.h"
#include "Checkpoint.h"
#include "Coin.h"
#include "Heart.h"
#include "Map.h"
#include "Collisions.h"
#include "Particles.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager(Render* render, Collisions* collisions, AudioManager* audio, Particles* particles) : Module()
{
	name.Create("entitymanager");
	this->collisions = collisions;
	this->render = render;
	this->audio = audio;
	this->particles = particles;
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
		case EntityType::PLAYER: ret = new Player(collisions,audio,this,particles); break;
		case EntityType::ENEMYFLY: ret = new EnemyFly(collisions,audio,this); break;
		case EntityType::ENEMYWALK: ret = new EnemyWalk(collisions, audio, this); break;
		case EntityType::COIN: ret = new Coin(collisions,audio,this); break;
		case EntityType::HEART: ret = new Heart(collisions,audio,this); break;
		case EntityType::CHECKPOINT: ret = new Checkpoint(collisions,audio,this); break;
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

// Load Game State
bool EntityManager::LoadState(pugi::xml_node& data)
{
	pugi::xml_node nodeEntity = data.child("entitymanager");
	for (int i = 0; i < entities.Count(); i++)
	{
		SString entityName;
		entityName.Create("Entity%d", i + 1);
		pugi::xml_node currentEntity = nodeEntity.child(entityName.GetString());
		entities.At(i)->data->position.x = currentEntity.attribute("x").as_int();
		entities.At(i)->data->position.y = currentEntity.attribute("y").as_int();
	}


	return true;
}

// Save Game State
bool EntityManager::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node nodeEntity = data.child("entitymanager");
	for (int i = 0; i < entities.Count(); i++)
	{
		SString entityName;
		entityName.Create("Entity%d", i + 1);
		pugi::xml_node currentEntity = nodeEntity.append_child(entityName.GetString());
		currentEntity.append_attribute("x").set_value(entities.At(i)->data->position.x);
		currentEntity.append_attribute("y").set_value(entities.At(i)->data->position.y);
	}

	return true;
}