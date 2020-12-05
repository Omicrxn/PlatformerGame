#include "Heart.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"


Heart::Heart() : Entity(EntityType::ENEMY_FLY)
{
	movingAnim.PushBack({ 0,0,16,16 });
	movingAnim.PushBack({ 16,0,16,16 });
	movingAnim.PushBack({ 32,0,16,16 });
	movingAnim.PushBack({ 48,0,16,16 });
	movingAnim.PushBack({ 64,0,16,16 });
	movingAnim.PushBack({ 80,0,16,16 });
	movingAnim.PushBack({ 96,0,16,16 });
	movingAnim.PushBack({ 112,0,16,16 });

	movingAnim.loop = true;
	movingAnim.speed = 0.09f;

	texture = app->tex->Load("Assets/items/heart.png");

	isLeft = true;

	initialPosition = { 1056, 813 };
	position = initialPosition;

	gravity = 1;
	velocity = { 0,0 };

	dead = false;
	collision = false;

}

bool Heart::Update(float dt)
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

