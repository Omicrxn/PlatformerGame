#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Module.h"
#include "Scene.h"

class App;
class Input;
class Render;
class Textures;
class Window;
class Collisions;
class AudioManager;

class SceneManager : public Module
{
public:

	SceneManager(Input* input, Render* render, Textures* tex, EntityManager* entityManager, Window* win, Collisions* collisions, AudioManager* audio, App* app);

	// Destructor
	virtual ~SceneManager();

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

	// Exit call coming from the menu
	bool menuExitCall = false;

	bool continueOption = false;

private:

	App* app = nullptr;
	Input* input = nullptr;
	Render* render = nullptr;
	Textures* tex = nullptr;
	EntityManager* entityManager = nullptr;
	Window* win = nullptr;
	Collisions* collisions = nullptr;
	AudioManager* audio = nullptr;

	Scene* current = nullptr;
	Scene* next = nullptr;

	// Required variables to manage screen transitions (fade-in, fade-out)
	bool onTransition = false;
	bool fadeOutCompleted = false;
	float transitionAlpha = 0.0f;
};

#endif // __SCENEMANAGER_H__