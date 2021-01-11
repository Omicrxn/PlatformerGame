#include "Checkpoint.h"
#include "Player.h"
#include "Audio.h"

Checkpoint::Checkpoint(Collisions* collisions, AudioManager* audio, EntityManager* entityManager) : Entity(EntityType::CHECKPOINT)
{
	name = "checkpoint";

	texture = NULL;
	checkpointAnimation.PushBack({ 20,7,35,57 });
	checkpointAnimation.PushBack({ 82,7,35,57 });
	checkpointAnimation.PushBack({ 145,7,35,57 });
	checkpointAnimation.PushBack({ 209,7,35,57 });

	checkpointAnimation.loop = false;
	checkpointAnimation.speed = 0.00f;

	this->audio = audio;
	fx = audio->LoadFx("Assets/Audio/Fx/checkpoint.wav");
	position = iPoint(0, 0);
	velocity = { 0,0 };
	scale = 3;
	collider = collisions->AddCollider({ position.x,position.y,35 * scale,57 * scale }, Collider::Type::ITEM_COIN, (Module*)entityManager);

	achieved = false;
}

bool Checkpoint::Update(float dt)
{
	bool ret = true;

	if (!pause)
	{
		// Update collider position
		if (collider != nullptr)
		{
			collider->SetPos(position.x, position.y);
		}
	}

	return ret;
}

void Checkpoint::Draw(Render* render)
{
	if (!pause)
	{
		// TODO: Calculate the corresponding rectangle depending on the
		// animation state and animation frame
		SDL_Rect rec = checkpointAnimation.GetFrame(3);
		render->scale = scale;
		render->DrawTexture(texture, position.x, position.y, &rec, 1.0f, true);
		render->scale = 1;
	}
}

void Checkpoint::SetPlayer(Player* player)
{
	this->player = player;
}

void Checkpoint::SetTexture(SDL_Texture* tex)
{
	texture = tex;
}

void Checkpoint::OnCollision(Collider* collider)
{
	if (!achieved)
	{
		audio->PlayFx(fx);
		player->lastCheckpointPos = { this->position.x - player->width,this->position.y };
		checkpointAnimation.speed = 0.04f;

		achieved = true;
	}
}