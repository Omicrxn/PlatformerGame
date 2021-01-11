#include "EnemyWalk.h"
#include "Pathfinding.h"

#define DEFAULT_PATH_LENGTH 50

EnemyWalk::EnemyWalk(Collisions* collisions, AudioManager* audio, EntityManager* entityManager) : Entity(EntityType::ENEMYWALK)
{
	name = "walkingenemy";
	pathfinding = PathFinding::GetInstance();

	for (int i = 0; i < 8; i++)
	{
		movingAnim.PushBack({ i * 32, 0, 32, 24 });
	}
	
	movingAnim.loop = true;
	movingAnim.speed = 0.1f;

	fx = audio->LoadFx("Assets/Audio/fx/enemy_explotion.wav");
	this->audio = audio;

	position = iPoint(20 * 64, -10 * 64);
	tempPosition = iPoint(20 * 64, -10 * 64);
	velocity = {0,0};
	dead = false;
	width = 32;
	height = 24;
	collider = collisions->AddCollider({ position.x, position.y, width, height }, Collider::Type::ENEMY, (Module*)entityManager);

	counter = 0;
}

EnemyWalk::~EnemyWalk()
{
}

bool EnemyWalk::Update(float dt)
{
	bool ret = true;

	if (!pause)
	{
#define GRAVITY 600

		tempPosition = position;

		if (!dead)
		{
			position.x = position.x + velocity.x * dt;
			position.y = position.y + velocity.y * dt + (GRAVITY * dt * dt * 0.5);
			velocity.y = velocity.y + GRAVITY * dt;
		}

		if (counter >= 0.27f)
		{
			counter = 0.0f;
			UpdatePath(map);
			Move(map);
		}
		counter += dt;

		// Update collider position
		if (collider != nullptr)
		{
			collider->SetPos(position.x, position.y);
		}
	}

	return ret;
}

void EnemyWalk::Move(Map* map)
{
	if (path.Count() > 0)
	{
		iPoint nextTile;
		path.Pop(nextTile);

		iPoint mapPos = map->WorldToMap(position.x, position.y);
		if (mapPos.x < nextTile.x)
			velocity.x = 150.0f;
		else
			velocity.x = -100.0f;
	}
	else
	{
		velocity = { 0,0 };
	}
}

void EnemyWalk::UpdatePath(Map* map)
{
	origin = map->WorldToMap(position.x, position.y);
	goal = map->WorldToMap(player->GetBounds().x, player->GetBounds().y);

	pathfinding->lastPath.Clear();
	if (pathfinding->CreatePath(origin, goal) != -1)
	{
		for (int i = 0; i < pathfinding->lastPath.Count(); i++)
		{
			path.PushBack(*pathfinding->lastPath.At(i));
		}
		path.Flip();
	}
}

void EnemyWalk::OnCollision(Collider* collision)
{
	audio->PlayFx(fx);
	player->score += 500;
	this->collider->pendingToDelete = true;
	active = false;
}

void EnemyWalk::Draw(Render* render)
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
		const DynArray<iPoint>* pathDebug = &pathfinding->lastPath;

		for (uint i = 0; i < pathDebug->Count(); ++i)
		{
			iPoint pos = { pathDebug->At(i)->x * 64, pathDebug->At(i)->y * 64 };
			render->DrawTexture(pathDebugTexture, pos.x, pos.y);
		}
	}
}

void EnemyWalk::SetTexture(SDL_Texture* tex, SDL_Texture* tex2)
{
	texture = tex;
	pathDebugTexture = tex2;
}

void EnemyWalk::SetPlayer(Player* player)
{
	this->player = player;
}

void EnemyWalk::SetMap(Map* map)
{
	this->map = map;
}