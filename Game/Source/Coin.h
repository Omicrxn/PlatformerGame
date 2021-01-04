#pragma once
#ifndef __COIN_H__
#define __COIN_H__

#include "Entity.h"
#include "Render.h"
#include "Collisions.h"
#include "EntityManager.h"

#include "Point.h"
#include "SString.h"

class Player;

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

public:

    SDL_Texture* texture;

    Animation coinAnimation;

    int fx;

    Player* player;
};

#endif // __COIN_H__