#include "Heart.h"
#include "Player.h"

Heart::Heart(Collisions* collisions, AudioManager* audio, EntityManager* entityManager) : Entity(EntityType::HEART)
{
	heartAnimation.PushBack({ 0,0,16,16 });
	heartAnimation.PushBack({ 16,0,16,16 });
	heartAnimation.PushBack({ 32,0,16,16 });
	heartAnimation.PushBack({ 48,0,16,16 });
	heartAnimation.PushBack({ 64,0,16,16 });
	heartAnimation.PushBack({ 80,0,16,16 });
	heartAnimation.PushBack({ 96,0,16,16 });
	heartAnimation.PushBack({ 112,0,16,16 });

	heartAnimation.loop = true;
	heartAnimation.speed = 0.09f;

	//fx = app->audio->LoadFx("Assets/Audio/Fx/item_pick.wav");

	position = iPoint(0, 0);
	velocity = { 0,0 };

	collider = collisions->AddCollider({ position.x,position.y,16*2,16*2 }, Collider::Type::ITEM_HEART, (Module*)entityManager);
}

bool Heart::Update(float dt)
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

void Heart::Draw(Render* render)
{
	if (!pause)
	{
		// TODO: Calculate the corresponding rectangle depending on the
		// animation state and animation frame
		SDL_Rect rec = heartAnimation.GetCurrentFrame();
		render->scale = 2;
		render->DrawTexture(texture, position.x, position.y, &rec, 1.0f);
		render->scale = 1;
	}
}

void Heart::SetPlayer(Player* player)
{
	this->player = player;
}

void Heart::SetTexture(SDL_Texture* tex)
{
	texture = tex;
}

void Heart::OnCollision(Collider* collider)
{
	//app->audio->PlayFx(fx);
	player->lifes++;
	printf("Lifes: %d \n", player->lifes);
	this->collider->pendingToDelete = true;
	active = false;
}