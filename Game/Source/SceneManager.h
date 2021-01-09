#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Module.h"
#include "Scene.h"

class GuiButton;

class Input;
class Render;
class Textures;
class Window;
class Collisions;

class SceneManager : public Module
{
public:

	SceneManager(Input* input, Render* render, Textures* tex, EntityManager* entityManager, Window* win, Collisions* collisions, AudioManager* audio);

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

private:

	Input* input;
	Render* render;
	Textures* tex;
	EntityManager* entityManager;
	Window* win;
	Collisions* collisions;
	AudioManager* audio;
	
	Scene* current;
	Scene* next;

	// Required variables to manage screen transitions (fade-in, fade-out)
	bool onTransition;
	bool fadeOutCompleted;
	float transitionAlpha;
};

#endif // __SCENEMANAGER_H__