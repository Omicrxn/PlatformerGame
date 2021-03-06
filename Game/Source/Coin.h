#pragma once
#ifndef __COIN_H__
#define __COIN_H__

#include "Entity.h"
#include "Render.h"

#include "EntityManager.h"
#include "Audio.h"
#include "Scene.h"
#include "SceneGameplay.h"

#include "Point.h"
#include "SString.h"

class Player;
class SceneGameplay;

class Coin : public Entity
{
public:

    Coin(Collisions* collisions, AudioManager* audio, EntityManager* entityManager);
    ~Coin();

    void SetTexture(SDL_Texture* tex);

    bool Update(float dt) override;

    void Draw(Render* render);

    void SetPlayer(Player* player);

    void OnCollision(Collider* collider) override;

    void SetScene(Scene* scene);

public:

    Animation coinAnimation;

    AudioManager* audio = nullptr;
    int fx = -1;

    Player* player = nullptr;

    SceneGameplay* sceneGameplay = nullptr;
};

#endif // __COIN_H__