#include "EnemyWalk.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
EnemyWalk::EnemyWalk() : Entity(EntityType::ENEMY_FLY)
{
	movingAnim.PushBack({ 0,1,18,16 });
	movingAnim.PushBack({ 18,1,18,16 });
	movingAnim.PushBack({ 36,1,18,16 });
	movingAnim.PushBack({ 54,1,18,16 });
	movingAnim.PushBack({ 72,1,18,16 });
	movingAnim.loop = true;
	movingAnim.speed = 0.09f;

	texture = app->tex->Load("Assets/enemies/crab.png");

	isLeft = true;

	initialPosition = { 760, 1327 };
	position = initialPosition;

	gravity = 1;
	velocity = { 0,0 };

	dead = false;
	collision = false;

}

bool EnemyWalk::Update(float dt)
{
	bool ret = true;
	if (current_anim != &movingAnim)
	{
		current_anim = &movingAnim;
		movingAnim.Reset();
	}
	rectAnim = current_anim->GetCurrentFrame();
	if (!app->render->DrawTexture(texture, position.x, position.y, &rectAnim, isLeft))
	{
		ret = false;
	}
	return ret;
}

void EnemyWalk::Move()
{

}