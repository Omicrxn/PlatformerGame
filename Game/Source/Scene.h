#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

struct SDL_Texture;
struct Player;
struct EnemyFly;
struct EnemyWalk;
struct Coin;
struct Heart;
struct Checkpoint;
class Scene : public Module
{
public:

	Scene(bool startEnabled);

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void DrawDebug();

	void VolumeUp();
	void VolumeDown();

	Player* player = nullptr;

	SDL_Texture* debugTex;

private:

	SDL_Texture* background1;
	SDL_Texture* background2;
	SDL_Texture* background3;
	SDL_Texture* background4;
	SDL_Rect backgroundRect;

	EnemyFly* flyingEnemy1 = nullptr;
	EnemyWalk* walkingEnemy1 = nullptr;

	Coin* coin = nullptr;
	Heart* heart = nullptr;
	Checkpoint* checkpoint = nullptr;

public:

	bool debugDraw = 0;
};

#endif // __SCENE_H__