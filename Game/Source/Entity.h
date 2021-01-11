#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Animation.h"

#include "Collisions.h"
class Render;
#include "SDL_image/include/SDL_image.h"

enum class EntityType
{
    PLAYER,
    ENEMYFLY,
    ENEMYWALK,
    COIN,
    HEART,
    MAP,
    CHECKPOINT,
    UNKNOWN
};

class Entity
{
public:

    Entity(EntityType type) : type(type), active(true) {}

    virtual bool Update(float dt)
    {
        return true;
    }

    virtual void Draw(Render* render) {};

    const Collider* GetCollider() const { return collider; }

    virtual void OnCollision(Collider* collider) {};

    virtual SDL_Rect GetBounds() const { return { collider->rect.x, collider->rect.y, width, height }; };

public:

    EntityType type = EntityType::UNKNOWN;

    bool active = true;
    //uint32 id;            // Entity identifier?

    Collider* collider = nullptr;
    SString name = "";
    // Possible properties, it depends on how generic we
    // want our Entity class, maybe it's not renderable...
    iPoint position = { 0, 0 };        // Use a float instead?
    fPoint velocity = { 0.0f, 0.0f };
    bool renderable = true;
    int width = 0, height = 0;
    SDL_Texture* texture = nullptr;

    iPoint tempPosition = { 0,0 };

    bool pause = false;

    // Entities attributes that need load / save
    // Player
    iPoint lastCheckpointPos = { 0, 0 };
    int lifes = 5;
    int score = 0;
};

#endif // __ENTITY_H__