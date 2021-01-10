#pragma once
#ifndef __ENEMYFLY_H__
#define __ENEMYFLY_H__

#include "Entity.h"
#include "Render.h"
#include "Audio.h"
#include "EntityManager.h"
#include "Map.h"
#include "Player.h"

#include "DynArray.h"
#include "Point.h"
#include "SString.h"

class PathFinding;

class EnemyFly : public Entity
{
public:

    EnemyFly(Collisions* collisions, AudioManager* audio, EntityManager* entityManager);
    virtual ~EnemyFly();

    bool Update(float dt) override;

    void UpdatePath(Map* map);

    void OnCollision(Collider* collider);

    void Draw(Render* render);

    void SetTexture(SDL_Texture* tex, SDL_Texture* tex2);
    void SetPlayer(Player* player);
    void SetMap(Map* map);

    void Move(Map* map);

    SDL_Texture* pathDebugTexture;
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

#endif // __ENEMYFLY_H__