#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Animation.h"

class Collider;
class Render;

enum class EntityType
{
    PLAYER,
    ENEMY,
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

public:

    EntityType type;

    bool active = true;

    //SString name;         // Entity name identifier?
    //uint32 id;            // Entity identifier?

    Collider* collider = nullptr;

    // Possible properties, it depends on how generic we
    // want our Entity class, maybe it's not renderable...
    iPoint position;        // Use a float instead?
    fPoint velocity;
    bool renderable = false;

   // SDL_Texture* texture;
};

#endif // __ENTITY_H__