#include "SceneManager.h"

#include "SceneLogo.h"
#include "SceneTitle.h"
#include "SceneGameplay.h"
#include "SceneEnding.h"

#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"
#include "Collisions.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"

#define FADEOUT_TRANSITION_SPEED	2.0f
#define FADEIN_TRANSITION_SPEED		2.0f

SceneManager::SceneManager(Input* input, Render* render, Textures* tex, EntityManager* entityManager, Window* win, Collisions* collisions, AudioManager* audio, App* app, GuiManager* guiManager) : Module()
{
	name.Create("scenemanager");

	onTransition = false;
	fadeOutCompleted = false;
	transitionAlpha = 0.0f;;

	this->app = app;
	this->input = input;
	this->render = render;
	this->tex = tex;
	this->entityManager = entityManager;
	this->win = win;
	this->collisions = collisions;
	this->audio = audio;
	this->guiManager = guiManager;
}

// Destructor
SceneManager::~SceneManager()
{}

// Called before render is available
bool SceneManager::Awake()
{
	LOG("Loading Scene manager");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneManager::Start()
{
	current = new SceneLogo();
	//current = new SceneTitle(win, this, audio, render, app, guiManager);

	if (current->name == "Gameplay")
	{
		current->Load(tex, entityManager);
	}
	else
	{
		current->Load(tex);	// Load next screen
	}

	next = nullptr;

	return true;
}

// Called each loop iteration
bool SceneManager::PreUpdate()
{
	/*
	// L12b: Debug pathfing
	static iPoint origin;
	static bool originSelected = false;

	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);
	iPoint p = app->render->ScreenToWorld(mouseX, mouseY);
	p = app->map->WorldToMap(p.x, p.y);

	if(app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if(originSelected == true)
		{
			app->pathFinding->CreatePath(origin, p);
			originSelected = false;
		}
		else
		{
			origin = p;
			originSelected = true;
		}
	}
	*/
	if (input->GetKey(SDL_SCANCODE_F1) == KeyState::KEY_DOWN)
	{
		current->TransitionToScene(SceneType::GAMEPLAY);
	}

	return true;
}

// Called each loop iteration
bool SceneManager::Update(float dt)
{
	if (!onTransition)
	{
		if (current->name == "Gameplay")
		{
			current->Update(input, collisions, dt);
		}
		else
		{
			current->Update(input, dt);
		}
	}
	else
	{
		if (!fadeOutCompleted)
		{
			transitionAlpha += (FADEOUT_TRANSITION_SPEED * dt);

			// NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
			// For that reason we compare against 1.01f, to avoid last frame loading stop
			if (transitionAlpha > 1.01f)
			{
				transitionAlpha = 1.0f;

				current->Unload(tex,audio);	// Unload current screen
				if (current->nextScene == SceneType::GAMEPLAY)
				{
					next->Load(tex, entityManager);
				}
				else
				{
					next->Load(tex);	// Load next screen
				}
				

				RELEASE(current);	// Free current pointer
				current = next;		// Assign next pointer
				next = nullptr;

				// Activate fade out effect to next loaded screen
				fadeOutCompleted = true;
			}
		}
		else  // Transition fade out logic
		{
			transitionAlpha -= (FADEIN_TRANSITION_SPEED * dt);

			if (transitionAlpha < -0.01f)
			{
				transitionAlpha = 0.0f;
				fadeOutCompleted = false;
				onTransition = false;
			}
		}
	}

	// Draw current scene
	current->Draw(render);

	// Draw full screen rectangle in front of everything
	if (onTransition)
	{
		render->DrawRectangle({ 0, 0, 1280, 720 }, { 0, 0, 0, (unsigned char)(255.0f * transitionAlpha) });
	}

	// L12b: Debug pathfinding
	/*
	app->input->GetMousePosition(mouseX, mouseY);
	iPoint p = app->render->ScreenToWorld(mouseX, mouseY);
	p = app->map->WorldToMap(p.x, p.y);
	p = app->map->MapToWorld(p.x, p.y);

	const DynArray<iPoint>* path = app->pathFinding->GetLastPath();

	for(uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(debugTex, pos.x, pos.y);
	}
	*/

	if (current->transitionRequired)
	{
		onTransition = true;
		fadeOutCompleted = false;
		transitionAlpha = 0.0f;

		switch (current->nextScene)
		{
			case SceneType::LOGO: next = new SceneLogo(); break;
			case SceneType::TITLE: next = new SceneTitle(win, this, audio, render, app, guiManager); break;
			case SceneType::GAMEPLAY: next = new SceneGameplay(app, this, win); break;
			case SceneType::ENDING: next = new SceneEnding(win); break;
			default: break;
		}

		current->transitionRequired = false;
	}

	if (input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || menuExitCall == true) return false;
	return true;
}

// Called each loop iteration
bool SceneManager::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool SceneManager::CleanUp()
{
	LOG("Freeing scene");

	if (current != nullptr) current->Unload(tex,audio);

	return true;
}