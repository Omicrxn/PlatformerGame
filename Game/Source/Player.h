#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Input.h"
#include "Render.h"
#include "Collisions.h"
#include "EntityManager.h"

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
    SHOOTING
};

class Player: public Entity
{
public:

    Player(Collisions* collisions, AudioManager* audio, EntityManager* entityManager);

    bool Update(Input* input, float dt);

    void Draw(Render* render);

    void SetTexture(SDL_Texture *tex);

    SDL_Rect GetBounds();

    void OnCollision(Collider* collider) override;

    // Movement functions
    void Run(bool isLeft);
    void Jump();
    void SmallJump();

public:

    SDL_Texture* texture;

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

    int width, height;

    bool readyToJump = true;
    bool hitObstacle = false;
    bool dead = false;
    bool godMode = false;
    bool isLeft = false;
    
    int lifes;
    int score;
    iPoint lastCheckpointPos = iPoint(0, 0);
};

#endif // __PLAYER_H__