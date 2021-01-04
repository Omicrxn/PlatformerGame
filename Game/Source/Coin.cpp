#include "Coin.h"
#include "Player.h"

Coin::Coin(Collisions* collisions, AudioManager* audio, EntityManager* entityManager) : Entity(EntityType::COIN)
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

bool Coin::Update(float dt)
{
	bool ret = true;

	// Update collider position
	if (collider != nullptr)
	{
		collider->SetPos(position.x, position.y);
	}

	return ret;
}

void Coin::Draw(Render* render)
{
	// TODO: Calculate the corresponding rectangle depending on the
	// animation state and animation frame
	SDL_Rect rec = coinAnimation.GetCurrentFrame();
	render->scale = 2;
	render->DrawTexture(texture, position.x, position.y, &rec, 1.0f);
	render->scale = 1;

}

void Coin::SetPlayer(Player* player)
{
	this->player = player;
}

void Coin::SetTexture(SDL_Texture* tex)
{
	texture = tex;
}

void Coin::OnCollision(Collider* collider)
{
	//app->audio->PlayFx(fx);
	player->score += 100;
	printf("Score: %d \n", player->score);
	this->collider->pendingToDelete = true;
	active = false;
}