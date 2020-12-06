#include "Particles.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Collisions.h"
#include "Scene.h"
#include "Player.h"
#include "Log.h"

#include "SDL/include/SDL_timer.h"

Particles::Particles(bool startEnabled) : Module(startEnabled)
{
	name = "particles";

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		particles[i] = nullptr;
}

Particles::~Particles()
{

}

bool Particles::Start()
{
	LOG("Loading bullet");
	bulletTexture = app->tex->Load("Assets/Player/bullet.png");

	// Enemy Bullet Animaton
	playerBullet.anim.PushBack({ 0, 0, 16, 16 });
	playerBullet.anim.PushBack({ 17, 0, 16, 16 });
	playerBullet.anim.loop = true;
	playerBullet.anim.speed = 0.075f;
	playerBullet.lifetime = 180;
	playerBullet.speed.x = 10;

	return true;
}

bool Particles::CleanUp()
{
	LOG("Unloading particles");

	// Delete all remaining active particles on application exit 
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (particles[i] != nullptr)
		{
			delete particles[i];
			particles[i] = nullptr;
			--particlesCount;
		}
	}

	app->tex->Unload(bulletTexture);

	return true;
}

void Particles::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		// Always destroy particles that collide
		if (particles[i] != nullptr && particles[i]->collider == c1)
		{
			delete particles[i];
			particles[i] = nullptr;
			--particlesCount;
			break;
		}
	}
}

bool Particles::Update(float dt)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if (particle == nullptr)	continue;

		// Call particle Update. If it has reached its lifetime, destroy it
		if (particle->Update() == false)
		{
			delete particle;
			particles[i] = nullptr;
			--particlesCount;
		}
	}
	
	return true;
}

bool Particles::PostUpdate()
{
	//Iterating all particle array and drawing any active particles
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if (particle != nullptr && particle->isAlive)
		{
			app->render->DrawTexture(bulletTexture, particle->position.x, particle->position.y, &(particle->anim.GetCurrentFrame()));
			/*if (particle->isWeapon)
			{
				app->render->DrawTexture(weaponsParticlesTexture, particle->position.x, particle->position.y, &(particle->anim.GetCurrentFrame()));

			}
			else
			{
				app->render->DrawTexture(particlesTexture, particle->position.x, particle->position.y, &(particle->anim.GetCurrentFrame()));
			}*/
		}
	}

	return true;
}

void Particles::AddParticle(const Particle& particle, int x, int y, Collider::Type colliderType, uint delay)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		//Finding an empty slot for a new particle
		if (particles[i] == nullptr)
		{
			Particle* p = new Particle(particle);

			p->frameCount = -(int)delay;			// We start the frameCount as the negative delay
			p->position.x = x;						// so when frameCount reaches 0 the particle will be activated
			p->position.y = y;

			//Adding the particle's collider
			if (colliderType != Collider::Type::NONE) {
				p->collider = app->collisions->AddCollider(p->anim.GetCurrentFrame(), colliderType, this);
			}

			particles[i] = p;
			++particlesCount;
			break;
		}
	}
}

Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) : anim(p.anim), position(p.position), speed(p.speed),
frameCount(p.frameCount), lifetime(p.lifetime),isWeapon(p.isWeapon)
{

}
Particle::~Particle()
{
	if (collider != nullptr)
		collider->pendingToDelete = true;
}
bool Particle::Update()
{
	bool ret = true;
	frameCount++;

	// The particle is set to 'alive' when the spawnTime is reached
	if (!isAlive && frameCount >= 0)
		isAlive = true;

	if (isAlive)
	{
		anim.Update();

		// If the particle has a specific lifetime, check when it has to be destroyed
		if (lifetime > 0)
		{
			if (frameCount >= lifetime)
				ret = false;
		}
		// Otherwise the particle is destroyed when the animation is finished
		else if (anim.Finished())
			ret = false;

		// Update the position in the screen
		position.x += speed.x;
		position.y += speed.y;

		if (collider != nullptr)
			collider->SetPos(position.x, position.y);
	}

	return ret;
}