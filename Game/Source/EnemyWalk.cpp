#include "EnemyWalk.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Map.h"
#include "Pathfinding.h"
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

	gravity = 1;
	velocity = { 0,0 };

	dead = false;
	collision = false;

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

bool EnemyWalk::Update(float dt)
{
	bool ret = true;
	if (!dead)
	{
		/*velocity.y += gravity;*/
		position.y += velocity.y * dt;
		/*isLeft ? position.x -= velocity.x : position.x += velocity.x;*/
		position.x += velocity.x * dt;
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

	if (counter >= 1.0f)
	{
		counter = 0.0f;
		Move();
	}
	counter += dt;

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
			this->velocity.y = gravity;
		else
			this->velocity.y = 0;
	}
	else
	{
		velocity = { 0,0 };
	}
}