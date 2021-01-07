#include "Player.h"

Player::Player(Collisions* collisions, AudioManager* audio, EntityManager* entityManager) : Entity(EntityType::PLAYER)
{
    texture = NULL;

    velocity = { 0,0 };

    // Define Player animations
    // Idle Animation
    for (int i = 0; i < 231 * 6; i += 231)
    {
        idleAnim.PushBack({ i,0,231,190 });
    }
    idleAnim.speed = 0.1f;

    // Running Animation
    for (int i = 0; i < 231 * 8; i += 231)
    {
        runningAnim.PushBack({ i,191,231,190 });
    }
    runningAnim.speed = 0.18f;

    // Jumping Animation
    for (int i = 0; i < 231 * 2; i += 231)
    {
        jumpingAnim.PushBack({ i,380,231,190 });
    }
    jumpingAnim.speed = 0.18f;

    // Falling Animation
    for (int i = 0; i < 231 * 2; i += 231)
    {
        fallingAnim.PushBack({ i,570,231,190 });
    }
    fallingAnim.speed = 0.18f;

    // Hit Animation
    for (int i = 0; i < 231 * 4; i += 231)
    {
        hitAnim.PushBack({ i,760,231,190 });
    }
    hitAnim.speed = 0.08f;

    // Dead Animation
    for (int i = 0; i < 231 * 7; i += 231)
    {
        deadAnim.PushBack({ i,950,231,190 });
    }
    deadAnim.speed = 0.08f;

    // Melee Animation
    for (int i = 0; i < 231 * 8; i += 231)
    {
        meleeAnim.PushBack({ i,1140,231,190 });
    }
    meleeAnim.speed = 0.08f;

    // Shooting Animation
    for (int i = 0; i < 231 * 8; i += 231)
    {
        shootingAnim.PushBack({ i,1330,231,190 });
    }
    shootingAnim.speed = 0.08f;

    width = 57;
    height = 86;
    position = iPoint(96,2300);
    score = 0;
    lifes = 5;
    collider = collisions->AddCollider({ position.x+86,position.y+43,width,height }, Collider::Type::PLAYER, (Module*)entityManager);
}

bool Player::Update(Input* input, float dt)
{
    #define GRAVITY 600.0f

    // Calculate movement
    if (!dead && !godMode)
    {
        position.x = position.x + velocity.x * dt;
        position.y = position.y + velocity.y * dt + (GRAVITY * dt * dt * 0.5);
        velocity.y = velocity.y + GRAVITY * dt;
    }

    if (readyToJump && !dead)
    {
        currentAnim = PlayerAnim::IDLE;
        velocity.x = 0;
    }
    else
    {
        currentAnim = PlayerAnim::FALL;
    }

    if (input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT);
    if (input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) Run(true);
    if (input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) Run(false);
    if (input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) Jump();
    if (input->GetKey(SDL_SCANCODE_UP) == KEY_UP) SmallJump();
    if (input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
    {
        currentAnim = PlayerAnim::SHOOTING;
    }

    // Update collider position
    if (collider != nullptr)
    {
        collider->SetPos(position.x + 86, position.y + 43);
    }

    return true;
}

void Player::Draw(Render* render)
{
    SDL_Rect rec;
    switch (currentAnim)
    {
    case PlayerAnim::IDLE:
        rec = idleAnim.GetCurrentFrame();
        break;
    case PlayerAnim::WALK:
        rec = runningAnim.GetCurrentFrame();
        break;
    case PlayerAnim::JUMP:
        rec = jumpingAnim.GetCurrentFrame();
        break;
    case PlayerAnim::FALL:
        rec = fallingAnim.GetCurrentFrame();
        break;    
    case PlayerAnim::SHOOTING:
        rec = shootingAnim.GetCurrentFrame();
        break;
    default:
        break;
    }
    render->DrawTexture(texture, position.x, position.y+15, &rec, 1.0f, isLeft);
}

void Player::SetTexture(SDL_Texture *tex)
{
    texture = tex;
}

SDL_Rect Player::GetBounds()
{
    return { position.x + 86, position.y + 43, width, height };
}

void Player::Run(bool isLeft)
{
    this->isLeft = isLeft;
    if (godMode)
    {
        isLeft ? position.x -= 3 : position.x += 3;
    }
    else
    {
        if(readyToJump)currentAnim = PlayerAnim::WALK;
        
        isLeft ? velocity.x = -200.0f : velocity.x = 250.0f;
    }
}

void Player::Jump()
{
    if (godMode)
    {
        position.y -= 3;
    }
    else
    {
        currentAnim = PlayerAnim::JUMP;
        if (readyToJump)
        {
            velocity.y = -400.0f;
            readyToJump = false;
        }
    }
}

// Add acceleration to Y speed
void Player::SmallJump()
{
    if (velocity.y < -200.0f && !godMode) velocity.y = -200.0f;
}

void Player::OnCollision(Collider* collider)
{
	if (collider->type == Collider::Type::ENEMY)
	{
		//audio->PlayFx(fx);
		
		if (lifes > 0) 
		{
			lifes--;
		}
		else 
		{
            active = false;
		}
	}
}