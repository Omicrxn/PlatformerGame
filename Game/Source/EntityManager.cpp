#include "EntityManager.h"
#include "Collisions.h"

#include "Player.h"
#include "EnemyFly.h"
#include "EnemyWalk.h"
#include "Checkpoint.h"
#include "Coin.h"
#include "Heart.h"
#include "Map.h"
#include "Particles.h"
#include "Collisions.h"
#include "Textures.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager(Render* render, Textures* tex, Collisions* collisions, AudioManager* audio, Particles* particles) : Module()
{
	name.Create("entitymanager");
	this->collisions = collisions;
	this->render = render;
	this->audio = audio;
	this->particles = particles;
	this->tex = tex;
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake()
{
	LOG("Loading Entity Manager");
	bool ret = true;

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	if (!active) return true;
	ListItem<Entity*>* entity = entities.start;
	while (entity != nullptr)
	{
		if (!entity->data->active)
			DestroyEntity(entity->data);

		entity = entity->next;
	}
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
		if (doLogic)
			entity->data->Update(dt);

		entity->data->Draw(render);

		if (!entity->data->active)
			DestroyEntity(entity->data);

		entity = entity->next;
	}

	return true;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item = entities.At(entities.Find(entity));
	tex->UnLoad(entity->texture);
	entity->collider->pendingToDelete = true;
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
	for (int i = 0; i < entities.Count(); i++)
	{
		SString entityName;
		entityName.Create("Entity%d", i + 1);
		pugi::xml_node currentEntity = data.child(entityName.GetString());

		if (entities.At(i)->data->type == EntityType::CHECKPOINT)
		{
			entities.At(i)->data->position.x = currentEntity.attribute("x").as_int();
			entities.At(i)->data->position.y = currentEntity.attribute("y").as_int();
		}
		else if (entities.At(i)->data->type == EntityType::PLAYER)
		{
			entities.At(i)->data->position.x = currentEntity.attribute("x").as_int();
			entities.At(i)->data->position.y = currentEntity.attribute("y").as_int();
			entities.At(i)->data->lifes = currentEntity.attribute("lifes").as_int();
			entities.At(i)->data->score = currentEntity.attribute("score").as_int();

			entities.At(i)->data->lastCheckpointPos.x = currentEntity.attribute("last_checkpoint_x").as_int();
			entities.At(i)->data->lastCheckpointPos.y = currentEntity.attribute("last_checkpoint_y").as_int();
		}
		else
		{
			entities.At(i)->data->position.x = currentEntity.attribute("x").as_int();
			entities.At(i)->data->position.y = currentEntity.attribute("y").as_int();
		}
	}

	return true;
}

// Save Game State
bool EntityManager::SaveState(pugi::xml_node& data) const
{
	for (int i = 0; i < entities.Count(); i++)
	{
		SString entityName;
		entityName.Create("Entity%d", i + 1);
		pugi::xml_node currentEntity = data.append_child(entityName.GetString());

		if (entities.At(i)->data->type == EntityType::CHECKPOINT)
		{
			currentEntity.append_attribute("x").set_value(entities.At(i)->data->position.x);
			currentEntity.append_attribute("y").set_value(entities.At(i)->data->position.y);
		}
		else if (entities.At(i)->data->type == EntityType::PLAYER)
		{
			currentEntity.append_attribute("x").set_value(entities.At(i)->data->position.x);
			currentEntity.append_attribute("y").set_value(entities.At(i)->data->position.y);
			currentEntity.append_attribute("lifes").set_value(entities.At(i)->data->lifes);
			currentEntity.append_attribute("score").set_value(entities.At(i)->data->score);

			currentEntity.append_attribute("last_checkpoint_x").set_value(entities.At(i)->data->lastCheckpointPos.x);
			currentEntity.append_attribute("last_checkpoint_y").set_value(entities.At(i)->data->lastCheckpointPos.y);
		}
		else
		{
			currentEntity.append_attribute("x").set_value(entities.At(i)->data->position.x);
			currentEntity.append_attribute("y").set_value(entities.At(i)->data->position.y);
		}
	}

	return true;
}