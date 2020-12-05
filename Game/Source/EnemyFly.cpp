#include "EnemyFly.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
EnemyFly::EnemyFly() : Entity(EntityType::ENEMY_FLY)
{
	movingAnim.PushBack({ 1,3,13,8 });
	movingAnim.PushBack({ 18,3,13,9 });
	movingAnim.PushBack({ 35,3,13,11 });
	movingAnim.PushBack({ 51,3,13,10 });
	movingAnim.PushBack({ 64,3,13,8 });
	movingAnim.loop = true;
	movingAnim.speed = 0.09f;

	texture = app->tex->Load("Assets/enemies/Bat/bat.png");

	isLeft = true;

	initialPosition = { 725, 900 };
	position = initialPosition;

	gravity = 1;
	velocity = { 0,0 };

	dead = false;
	collision = false;

}

bool EnemyFly::Update(float dt) 
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

void EnemyFly::Move()
{

}