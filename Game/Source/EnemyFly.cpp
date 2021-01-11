#include "EnemyFly.h"
#include "Pathfinding.h"

#define DEFAULT_PATH_LENGTH 50

EnemyFly::EnemyFly(Collisions* collisions, AudioManager* audio, EntityManager* entityManager) : Entity(EntityType::ENEMYFLY)
{
	name = "flyingenemy";

	pathfinding = PathFinding::GetInstance();

	for (int i = 0; i < 5; i++)
	{
		movingAnim.PushBack({ i * 64, 0, 64, 64 });
	}
	movingAnim.loop = true;
	movingAnim.speed = 0.1f;

    fx = audio->LoadFx("Assets/Audio/fx/enemy_explotion.wav");
	this->audio = audio;

    position = { 0, 0 };

    dead = false;

    collider = collisions->AddCollider({ position.x, position.y, 32, 32 }, Collider::Type::ENEMY, (Module*)entityManager);

    counter = 0;
}

EnemyFly::~EnemyFly()
{
}

bool EnemyFly::Update(float dt)
{
	bool ret = true;

	if (!pause)
	{
		if (!dead)
		{
			position.x = position.x + velocity.x;
			position.y = position.y + velocity.y;
		}

		if (counter >= 0.15f)
		{
			counter = 0.0f;
			UpdatePath(map);
			Move(map);
		}
		counter += dt;

		// Update collider position
		if (collider != nullptr)
		{
			collider->SetPos(position.x + 15, position.y + 15);
		}
	}

	return ret;
}

void EnemyFly::Move(Map* map)
{
	if (path.Count() > 0)
	{
		iPoint nextTile;
		path.Pop(nextTile);

		nextTile = { nextTile.x * 64, nextTile.y * 64 };

		/*iPoint mapPos = map->WorldToMap(position.x, position.y);*/
		if (position.x < nextTile.x)
			velocity.x = 1;
		else
			velocity.x = -1;

		if (position.y < nextTile.y)
			velocity.y = 1;
		else
			velocity.y = -1;
	}
	else
	{
		velocity = { 0,0 };
	}
}

void EnemyFly::UpdatePath(Map* map)
{
	origin = map->WorldToMap(position.x, position.y);
	goal = map->WorldToMap(player->GetBounds().x, player->GetBounds().y);

	pathfinding->lastPath.Clear();
	if (pathfinding->CreatePath(origin, goal) != -1)
	{
		path.Clear();
		for (int i = 0; i < pathfinding->lastPath.Count(); i++)
		{
			path.PushBack(*pathfinding->lastPath.At(i));
		}
		path.Flip();
		path.Pop(iPoint());
		path.Pop(iPoint());
		path.Pop(iPoint());
	}
}

void EnemyFly::OnCollision(Collider* collision)
{
	audio->PlayFx(fx);
	player->score += 500;
	this->collider->pendingToDelete = true;
	active = false;
}

void EnemyFly::Draw(Render* render)
{
	if (!pause)
	{
		rectAnim = movingAnim.GetCurrentFrame();
		if (velocity.x > 0)
			isLeft = false;
		else
			isLeft = true;
		render->DrawTexture(texture, position.x, position.y, &rectAnim, 1.0f, isLeft);
	}

	if (debugDraw)
	{
		/*const DynArray<iPoint>* pathDebug = &pathfinding->lastPath;*/

		for (uint i = 0; i < path.Count(); ++i)
		{
			iPoint pos = { path.At(i)->x * 64, path.At(i)->y * 64 };
			render->DrawTexture(pathDebugTexture, pos.x, pos.y);
		}
	}
}

void EnemyFly::SetTexture(SDL_Texture* tex, SDL_Texture* tex2)
{
	texture = tex;
	pathDebugTexture = tex2;
}

void EnemyFly::SetPlayer(Player* player)
{
	this->player = player;
}

void EnemyFly::SetMap(Map* map)
{
	this->map = map;
}