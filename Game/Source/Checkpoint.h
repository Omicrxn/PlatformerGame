#pragma once
#ifndef __CHECKPOINT_H__
#define __CHECKPOINT_H__

#include "Entity.h"
#include "Render.h"
#include "Collisions.h"
#include "EntityManager.h"

#include "Point.h"
#include "SString.h"
class Player;
class Checkpoint : public Entity
{
public:

    Checkpoint(Collisions* collisions, EntityManager* entityManager);
    ~Checkpoint();
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

#endif // __CHECKPOINT_H__
