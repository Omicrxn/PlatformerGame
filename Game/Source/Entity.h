#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Animation.h"

#include "Collisions.h"
class Render;

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

    EntityType type;

    bool active = true;
    //uint32 id;            // Entity identifier?

    Collider* collider = nullptr;
    SString name;
    // Possible properties, it depends on how generic we
    // want our Entity class, maybe it's not renderable...
    iPoint position;        // Use a float instead?
    fPoint velocity;
    bool renderable = false;
    int width, height;
   // SDL_Texture* texture;
};

#endif // __ENTITY_H__