#include "Coin.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"

Coin::Coin() : Entity(EntityType::ENEMY_FLY)
{
	movingAnim.PushBack({ 0,0,16,16 });
	movingAnim.PushBack({ 16,0,16,16 });
	movingAnim.PushBack({ 32,0,16,16 });
	movingAnim.PushBack({ 48,0,16,16 });
	movingAnim.PushBack({ 63,0,16,16 });

	movingAnim.loop = true;
	movingAnim.speed = 0.09f;

	texture = app->tex->Load("Assets/items/coin.png");

	isLeft = true;

	initialPosition = { 1476, 907 };
	position = initialPosition;

	gravity = 1;
	velocity = { 0,0 };

	dead = false;
	collision = false;

}

bool Coin::Update(float dt)
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
