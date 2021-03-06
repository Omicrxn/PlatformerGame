#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Input.h"
#include "Render.h"
#include "EntityManager.h"
#include "Particles.h"

#include "Point.h"
#include "SString.h"

#include "SDL/include/SDL.h"

enum class PlayerAnim
{
    IDLE,
    WALK,
    JUMP,
    FALL,
    MELEE,
    SHOOTING,
    DEAD,
};

class Player: public Entity
{
public:

    Player(Collisions* collisions, AudioManager* audio, EntityManager* entityManager, Particles* particles);

    bool Update(Input* input, float dt);

    void Draw(Render* render);

    void SetTexture(SDL_Texture *tex);

    void OnCollision(Collider* collider) override;

    // Movement functions
    void Run(bool isLeft);
    void Jump();
    void SmallJump();
    void Die();
    void Shoot();
public:

    SDL_Rect animRec = { 0, 0, 0, 0 };

    // Define all animation properties
    Animation idleAnim;
    Animation runningAnim;
    Animation jumpingAnim;
    Animation hitAnim;
    Animation deadAnim;
    Animation fallingAnim;
    Animation meleeAnim;
    Animation shootingAnim;

    PlayerAnim currentAnim;

    Particles* particles = nullptr;

    bool readyToJump = true;
    bool hitObstacle = false;
    bool dead = false;
    bool hit = false;
    bool godMode = false;
    bool isLeft = false;

};

#endif // __PLAYER_H__