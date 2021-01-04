#include "Checkpoint.h"
#include "Player.h"

Checkpoint::Checkpoint(Collisions* collisions, AudioManager* audio, EntityManager* entityManager) : Entity(EntityType::CHECKPOINT)
{
	texture = NULL;
	checkpointAnimation.PushBack({ 20,7,23,57 });

	checkpointAnimation.loop = false;
	checkpointAnimation.speed = 0.00f;

	//fx = app->audio->LoadFx("Assets/Audio/Fx/item_pick.wav");
	position = iPoint(0, 0);
	velocity = { 0,0 };

	collider = collisions->AddCollider({ position.x,position.y,16,16 }, Collider::Type::ITEM_COIN, (Module*)entityManager);
}

bool Checkpoint::Update(float dt)
{
	bool ret = true;

	// Update collider position
	if (collider != nullptr)
	{
		collider->SetPos(position.x, position.y);
	}

	return ret;
}

void Checkpoint::Draw(Render* render)
{
	// TODO: Calculate the corresponding rectangle depending on the
	// animation state and animation frame
	SDL_Rect rec = checkpointAnimation.GetCurrentFrame();

	//render->scale = 1;
	render->DrawTexture(texture, position.x, position.y, &rec, 1.0f);
	//render->scale = 1;
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
	//app->audio->PlayFx(fx);
	player->lastCheckpointPos = this->position;
}