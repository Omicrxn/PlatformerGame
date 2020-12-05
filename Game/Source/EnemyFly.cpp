#include "EnemyFly.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Input.h"
#include "Map.h"
#include "Pathfinding.h"
#include "Player.h"
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

	initialPosition = { 725, 900 };
	position = initialPosition;

	gravity = 1;
	velocity = { 0,0 };

	dead = false;
	collision = false;

	counter = 0;
}

bool EnemyFly::Update(float dt) 
{
	bool ret = true;
	if (!dead)
	{
		velocity.y += gravity;
		position.y += velocity.y;
		isLeft ? position.x -= velocity.x : position.x += velocity.x;
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

	if (counter > 1000)
	{
		counter = 0;
		//Move();
	}
	counter += dt;

	return ret;
}

void EnemyFly::Move()
{
	origin = position;
	goal = app->scene->player->position;

	app->pathfinding->lastPath.Clear();
	app->pathfinding->CreatePath(origin, goal);
	path = app->pathfinding->lastPath;

	iPoint nextTile = { path.At(0)->x, path.At(0)->y };
	if (nextTile.x < position.x)
		velocity.x = -10;

	else if (nextTile.x > position.x)
		velocity.x;

	if (nextTile.y < position.y)
		velocity.y = -10;

	else if (nextTile.x > position.x)
		velocity.y = 10;
}