#include "Checkpoint.h"
#include "Player.h"


Checkpoint::Checkpoint(Collisions* collisions, EntityManager* entityManager) : Entity(EntityType::CHECKPOINT)
{
	texture = NULL;
	coinAnimation.PushBack({ 0,0,16,16 });
	coinAnimation.PushBack({ 16,0,16,16 });
	coinAnimation.PushBack({ 32,0,16,16 });
	coinAnimation.PushBack({ 48,0,16,16 });
	coinAnimation.PushBack({ 63,0,16,16 });

	coinAnimation.loop = true;
	coinAnimation.speed = 0.09f;

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
	SDL_Rect rec = coinAnimation.GetCurrentFrame();

	render->DrawTexture(texture, position.x, position.y, &rec, 1.0f);

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