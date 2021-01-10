#pragma once
#ifndef __HEART_H__
#define __HEART_H__

#include "Entity.h"
#include "Render.h"
#include "EntityManager.h"
#include "Scene.h"
#include "SceneGameplay.h"

#include "Point.h"
#include "SString.h"

class Player;
class SceneGameplay;

class Heart : public Entity
{
public:

	Heart(Collisions* collisions, AudioManager* audio, EntityManager* entityManager);

	~Heart();

	void SetTexture(SDL_Texture* tex);

	bool Update(float dt) override;

	void Draw(Render* render);

	void SetPlayer(Player* player);

	void OnCollision(Collider* collider) override;

	void SetScene(Scene* scene);

public:

	SDL_Texture* texture;

	Animation heartAnimation;

	AudioManager* audio;
	int fx;

	Player* player;

	SceneGameplay* sceneGameplay;
};
#endif //__HEART_H__