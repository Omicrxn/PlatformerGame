#ifndef __ENEMY_H__
#define __ENEMY_H__

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

class Enemy : public Entity
{
public:

    Enemy(Collisions* collisions, AudioManager* audio, EntityManager* entityManager);
    virtual ~Enemy();

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
    Animation* currentAnim;
    SDL_Rect rectAnim;

    bool dead;

    // Variables to manage the enemy path
    DynArray<iPoint>* path;
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

#endif // __ENEMY_H__