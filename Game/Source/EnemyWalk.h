#pragma once
#ifndef __ENEMYWALK_H__
#define __ENEMYWALK_H__

#include "Entity.h"
#include "Render.h"
#include "Collisions.h"
#include "Audio.h"
#include "EntityManager.h"
#include "Map.h"
#include "Player.h"

#include "DynArray.h"
#include "Point.h"
#include "SString.h"

class PathFinding;

class EnemyWalk : public Entity
{
public:

    EnemyWalk(Collisions* collisions, AudioManager* audio, EntityManager* entityManager);
    virtual ~EnemyWalk();

    bool Update(float dt) override;

    void UpdatePath(Map* map);

    void OnCollision(Collider* collider);

    void Draw(Render* render);

    void SetTexture(SDL_Texture* tex);
    void SetPlayer(Player* player);
    void SetMap(Map* map);

    void Move(Map* map);

    SDL_Texture* texture;
    Animation movingAnim;
    SDL_Rect rectAnim;

    bool dead;
    bool isLeft;

    // Variables to manage the enemy path
    DynArray<iPoint> path;
    iPoint origin;
    iPoint goal;
    float counter;

    // Enemy audio
    AudioManager* audio;
    int fx;

    Map* map;
    Player* player;
    PathFinding* pathfinding;
};

#endif // __ENEMYWALK_H__