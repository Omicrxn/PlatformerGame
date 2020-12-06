#include "EnemyWalk.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Map.h"
#include "Pathfinding.h"
#include "EntityManager.h"
#include "Player.h"
#include "Scene.h"

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

	initialPosition = { 760, 975 };
	position = initialPosition;

	gravity = 1.0f;
	velocity = { 0,0 };

	dead = false;
	collision = false;
	collider = app->collisions->AddCollider({ position.x,position.y,18,16 }, Collider::Type::ENEMY, (Module*)app->entityman);
	counter = 0;
}

bool EnemyWalk::Update(float dt)
{
	bool ret = true;
		if (!dead)
		{
			position.x = position.x + velocity.x * dt;
			position.y = position.y + velocity.y * dt + (gravity * dt * dt * 0.5);
			velocity.y = velocity.y + gravity * dt;
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

	if (counter >= 0.5f)
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

	/*DrawPath();*/

	return ret;
}

void EnemyWalk::Move()
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
			Fall();
		else
			Jump();
	}
	else
	{
		velocity = { 0,0 };
	}
}

void EnemyWalk::Fall()
{
	if (velocity.y > 0) velocity.y = 1.0f;
}

void EnemyWalk::Jump()
{
	velocity.y = -1;
}

void EnemyWalk::UpdatePath()
{
	origin = app->map->WorldToMap(position.x, position.y);
	goal = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);

	app->pathfinding->lastPath.Clear();
	if (app->pathfinding->CreatePath(origin, goal) != -1)
	{
		for (int i = 0; i < app->pathfinding->lastPath.Count(); i++)
		{
			path.PushBack(*app->pathfinding->lastPath.At(i));
		}
	}

	path.Flip();
}

void EnemyWalk::OnCollision(Collider* collider)
{
	app->scene->player->score += 300;
	app->scene->player->PrintData();
	app->entityman->DestroyEntity(this);
}

void EnemyWalk::DrawPath()
{
	if (app->pathfinding->debug == true)
	{
		for (uint i = 0; i < path.Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(path.At(i)->x, path.At(i)->y);
			app->render->DrawTexture(app->scene->debugTex, pos.x, pos.y);
		}
	}
}