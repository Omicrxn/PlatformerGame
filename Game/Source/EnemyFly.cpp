#include "EnemyFly.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Map.h"
#include "Pathfinding.h"
#include "Player.h"
#include "EntityManager.h"
#include "Scene.h"
#include "Audio.h"

EnemyFly::EnemyFly() : Entity(EntityType::ENEMY_FLY)
{
	movingAnim.PushBack({ 1,3,13,8 });
	movingAnim.PushBack({ 18,3,13,9 });
	movingAnim.PushBack({ 35,3,13,11 });
	movingAnim.PushBack({ 51,3,13,10 });
	movingAnim.PushBack({ 64,3,13,8 });
	movingAnim.loop = true;
	movingAnim.speed = 0.09f;

	texture = app->tex->Load("Assets/enemies/bat.png");
	fx = app->audio->LoadFx("Assets/audio/fx/enemy_explotion.wav");

	isLeft = true;

	initialPosition = { 200, 800 };
	position = initialPosition;

	gravity = 1;
	velocity = { 0,0 };

	dead = false;
	collision = false;
	collider = app->collisions->AddCollider({ position.x,position.y,13,11 }, Collider::Type::ENEMY, (Module*)app->entityman);

	counter = 0;
}

bool EnemyFly::Update(float dt) 
{
	bool ret = true;
	if (!dead)
	{
		position.x = position.x + velocity.x;
		position.y = position.y + velocity.y;
	}

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

	if (counter >= 0.27f)
	{
		counter = 0.0f;
		UpdatePath();
		Move();
	}
	counter += dt;

	// Update collider position
	if (collider != nullptr) {
		collider->SetPos(position.x, position.y);
	}

	DrawPath();

	return ret;
}

void EnemyFly::Move()
{
	if (path.Count() > 0)
	{
		iPoint nextTile;
		path.Pop(nextTile);

		iPoint mapPos = app->map->WorldToMap(position.x, position.y);
		if (mapPos.x < nextTile.x)
			this->velocity.x = 1;
		else
			this->velocity.x = -1;

		if (mapPos.y < nextTile.y)
			this->velocity.y = 1;
		else
			this->velocity.y = -1;
	}
	else
	{
		velocity = { 0,0 };
	}
}


void EnemyFly::UpdatePath()
{
	origin = app->map->WorldToMap(position.x, position.y);
	goal = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);

	if (abs(origin.x - goal.x) < 10 && abs(origin.y - goal.y) < 10)
	{
		app->pathfinding->lastPath.Clear();
		if (app->pathfinding->CreatePath(origin, goal) != -1)
		{
			for (int i = 0; i < app->pathfinding->lastPath.Count(); i++)
			{
				path.PushBack(*app->pathfinding->lastPath.At(i));
			}
			path.Flip();
		}
	}
}

void EnemyFly::OnCollision(Collider* collider)
{
		//app->scene->player->score += scoreGiven;
		app->audio->PlayFx(fx);
		app->scene->player->score += 500;
		app->scene->player->PrintData();
		app->entityman->DestroyEntity(this);
}

void EnemyFly::DrawPath()
{
	if (app->pathfinding->debug == true)
	{
		const DynArray<iPoint>* path = app->pathfinding->GetLastPath();

		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			app->render->DrawTexture(app->scene->debugTex, pos.x, pos.y);
		}
	}
}