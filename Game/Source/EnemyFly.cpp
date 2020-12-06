#include "EnemyFly.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Map.h"
#include "Pathfinding.h"
#include "Player.h"
#include "EntityManager.h"
#include "Scene.h"

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

	isLeft = true;

	initialPosition = { 100, 800 };
	position = initialPosition;

	gravity = 1;
	velocity = { 0,0 };

	dead = false;
	collision = false;

	collider = app->collisions->AddCollider({ position.x,position.y,13,11 }, Collider::Type::ENEMY, (Module*)app->entityman);

	counter = 0;

	origin = app->map->WorldToMap(position.x, position.y);
	goal = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);

	app->pathfinding->lastPath.Clear();
	app->pathfinding->CreatePath(origin, goal);
	for (int i = 0; i < app->pathfinding->lastPath.Count(); i++)
	{
		path.PushBack(*app->pathfinding->lastPath.At(i));
	}
	path.Flip();
}

bool EnemyFly::Update(float dt) 
{
	bool ret = true;
	if (!dead)
	{
		velocity.y += gravity;
		/*position.y += velocity.y*dt;*/
		isLeft ? position.x -= velocity.x : position.x += velocity.x;
		/*position.x += velocity.x*dt;*/
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
		Move();
	}
	counter += dt;
	// Update collider position
	if (collider != nullptr) {
		collider->SetPos(position.x, position.y);
	}
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

void EnemyFly::OnCollision(Collider* collider)
{
	//app->scene->player->score += scoreGiven;

	app->entityman->DestroyEntity(this);
}