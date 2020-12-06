#include "Coin.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "EntityManager.h"
#include "Scene.h"
#include "Audio.h"
#include "Player.h"

Coin::Coin() : Entity(EntityType::ENEMY_FLY)
{
	movingAnim.PushBack({ 0,0,16,16 });
	movingAnim.PushBack({ 16,0,16,16 });
	movingAnim.PushBack({ 32,0,16,16 });
	movingAnim.PushBack({ 48,0,16,16 });
	movingAnim.PushBack({ 63,0,16,16 });

	movingAnim.loop = true;
	movingAnim.speed = 0.09f;

	texture = app->tex->Load("Assets/Items/coin.png");
	fx = app->audio->LoadFx("Assets/Audio/Fx/item_pick.wav");
	isLeft = true;

	initialPosition = { 1476, 907 };
	position = initialPosition;

	gravity = 1;
	velocity = { 0,0 };

	dead = false;
	collision = false;
	collider = app->collisions->AddCollider({ position.x,position.y,16,16 }, Collider::Type::ITEM_COIN, (Module*)app->entityman);
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

	// Update collider position
	if (collider != nullptr) {
		collider->SetPos(position.x, position.y);
	}

	return ret;
}

void Coin::OnCollision(Collider* collider)
{

	app->audio->PlayFx(fx);
	app->scene->player->score += 100;
	app->scene->player->PrintData();
  	app->entityman->DestroyEntity(this);
	

}
