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

    SDL_Texture* pathDebugTexture = nullptr;
    Animation movingAnim;
    SDL_Rect rectAnim = { 0, 0, 0, 0, };

    bool dead = false;
    bool isLeft = false;

    // Variables to manage the enemy path
    DynArray<iPoint> path;
    iPoint origin = { 0, 0 };
    iPoint goal = { 0, 0 };
    float counter = 0.0f;

    // Enemy audio
    AudioManager* audio = nullptr;
    int fx = -1;

    Map* map = nullptr;
    Player* player = nullptr;
    PathFinding* pathfinding = nullptr;

    bool debugDraw = false;
};

#endif // __ENEMYFLY_H__