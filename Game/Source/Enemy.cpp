#include "Enemy.h"
#include "Pathfinding.h"

#define DEFAULT_PATH_LENGTH 50

Enemy::Enemy(Collisions* collisions, AudioManager* audio, EntityManager* entityManager) : Entity(EntityType::ENEMY)
{
	pathfinding = PathFinding::GetInstance();

    //movingAnim.PushBack({});
    movingAnim.loop = true;
    movingAnim.speed = 0.1;

    fx = audio->LoadFx("Assets/Áudio/fx/enemy_explotion.wav");
	this->audio = audio;

    position = { 0, 0 };

    dead = false;

    collider = collisions->AddCollider({ position.x,position.y,13,11 }, Collider::Type::ENEMY, (Module*)entityManager);

    counter = 0;
}

Enemy::~Enemy()
{
}

bool Enemy::Update(float dt)
{
	bool ret = true;

	if (!dead)
	{
		position.x = position.x + velocity.x;
		position.y = position.y + velocity.y;
	}

	if (currentAnim != &movingAnim)
	{
		currentAnim = &movingAnim;
		movingAnim.Reset();
	}

	rectAnim = currentAnim->GetCurrentFrame();

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

	return ret;
}

void Enemy::Move(Map* map)
{
	if (path.Count() > 0)
	{
		iPoint nextTile;
		path.Pop(nextTile);

		iPoint mapPos = map->WorldToMap(position.x, position.y);
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

void Enemy::UpdatePath(Map* map)
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

void Enemy::OnCollision(Collider* collision)
{
	audio->PlayFx(fx);
	player->score += 500;
	this->collider->pendingToDelete = true;
	active = false;
}

void Enemy::Draw(Render* render)
{
	render->DrawTexture(texture, position.x, position.y, &rectAnim, false);
}

void Enemy::SetTexture(SDL_Texture* tex)
{
	texture = tex;
}

void Enemy::SetPlayer(Player* player)
{
	this->player = player;
}

void Enemy::SetMap(Map* map)
{
	this->map = map;
}