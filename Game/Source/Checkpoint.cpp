#include "Checkpoint.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "EntityManager.h"
#include "Map.h"
#include "Scene.h"
#include "Audio.h"
#include "Player.h"

Checkpoint::Checkpoint() : Entity(EntityType::CHECKPOINT)
{
	redAnim.PushBack({ 0,16,16,16 });
	redAnim.PushBack({ 16,16,16,16 });
	redAnim.PushBack({ 32,16,16,16 });
	redAnim.PushBack({ 48,16,16,16 });

	blueAnim.PushBack({ 0,32,16,16 });
	blueAnim.PushBack({ 16,32,16,16 });
	blueAnim.PushBack({ 32,32,16,16 });
	blueAnim.PushBack({ 48,32,16,16 });

	redAnim.loop = blueAnim.loop = true;
	redAnim.speed = blueAnim.speed = 0.05f;

	texture = app->tex->Load("Assets/items/checkpoints.png");
	fx = app->audio->LoadFx("Assets/audio/fx/checkpoint.wav");
	isLeft = true;

	initialPosition = { 944, 1328 };
	position = initialPosition;

	//checkpoints[1] = { 59, 83 };
	//checkpoints[2] = { 83, 59 };
	//checkpoints[3] = { 121, 14 };

	gravity = 1;
	velocity = { 0,0 };

	dead = false;
	collision = false;
	collider = app->collisions->AddCollider({ position.x,position.y,16,16 }, Collider::Type::CHECKPOINT, (Module*)app->entityman);

	// Update collider position
	if (collider != nullptr)
	{
		collider->SetPos(position.x, position.y);
	}
}

bool Checkpoint::Update(float dt)
{
	bool ret = true;

	if (!app->scene->player->hasCheckpoint)
	{
		if (current_anim != &redAnim)
		{
			current_anim = &redAnim;
			redAnim.Reset();
		}
	}
	else
	{
		if (current_anim != &blueAnim)
		{
			current_anim = &blueAnim;
			blueAnim.Reset();
		}
	}
	rectAnim = current_anim->GetCurrentFrame();

	if (!app->render->DrawTexture(texture, position.x, position.y, &rectAnim, isLeft))
	{
		ret = false;
	}

	return ret;
}

void Checkpoint::OnCollision(Collider* collider)
{
	if (!app->scene->player->hasCheckpoint)
	{
		app->scene->player->hasCheckpoint = true;
		app->audio->PlayFx(fx);
	}
}
