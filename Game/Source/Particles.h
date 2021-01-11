#pragma once
#ifndef __PARTICLES_H__
#define __PARTICLES_H__

#include "App.h"
#include "Animation.h"
#include "Point.h"
#include "Module.h"
#include "SDL/include/SDL.h"
#include "Collisions.h"

#define MAX_ACTIVE_PARTICLES 200

struct SDL_Texture;

struct Collider;

struct Particle
{
public:
	// Constructor
	Particle();

	// Copy constructor
	Particle(const Particle& p);

	~Particle();

	// Called in ModuleParticles' Update
	// Handles the logic of the particle
	// Returns false when the particle reaches its lifetime
	bool Update();

public:
	// Defines the position in the screen
	iPoint position = { 0, 0 };

	// Defines the speed at which the particle will move (pixels per second)
	iPoint speed = { 0, 0 };

	// A set of rectangle sprites
	Animation anim;

	// Defines wether the particle is alive or not
	// Particles will be set to not alive until "spawnTime" is reached
	bool isAlive = false;

	// Defines the time when the particle will be spawned
	int frameCount = 0;

	// Defines the total amount of time during which the particle will be active (in miliseconds)
	Uint32 lifetime = 0;

	// The particle's collider
	Collider* collider = nullptr;
};

class Textures;
class Render;
class Collisions;

class Particles : public Module
{
public:
	// Constructor
	// Initializes all the particles in the array to nullptr
	Particles(Textures* tex, Render* render, Collisions* collisions);

	//Destructor
	~Particles();

	// Called when the module is activated
	// Loads the necessary textures for the particles
	bool Start() override;

	// Called at the middle of the application loop
	// Iterates all the particles and calls its Update()
	// Removes any "dead" particles
	bool Update(float dt) override;

	// Called at the end of the application loop
	// Iterates all the particles and draws them
	bool PostUpdate() override;

	// Called on application exit
	// Destroys all active particles left in the array
	bool CleanUp() override;

	// Called when a particle collider hits another collider
	void OnCollision(Collider* c1, Collider* c2) override;

	// Creates a new particle and adds it to the array
	// Param particle	- A template particle from which the new particle will be created
	// Param x, y		- Position x,y in the screen (upper left axis)
	// Param delay		- Delay time from the moment the function is called until the particle is displayed in screen
	void AddParticle(const Particle& particle, int x, int y, Collider::Type colliderType = Collider::Type::NONE, uint delay = 0);

	inline uint GetParticlesCount() const { return particlesCount; };

private:

	// Bullet spritesheet loaded into an SDL Texture
	SDL_Texture* bulletTexture = nullptr;

	// An array to store and handle all the particles
	Particle* particles[MAX_ACTIVE_PARTICLES] = { nullptr };

	// An index to the last added particle
	uint lastParticle = 0;

	// Total amount of particles loaded into the array
	uint particlesCount = 0;

	Textures* tex = nullptr;

	Render* render = nullptr;

	Collisions* collisions = nullptr;

public:
	// Template particle for an explosion
	Particle playerBullet;
};

#endif // __PARTICLES_H__