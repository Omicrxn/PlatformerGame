#include "App.h"
#include "Window.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "LogoScreen.h"
#include "TitleScreen.h"
#include "EndingScreen.h"
#include "Render.h"
#include "Textures.h"
#include "Pathfinding.h"
#include "Audio.h"
#include "Collisions.h"
#include "Scene.h"
#include "Map.h"
#include "EntityManager.h"
#include "InputHandler.h"
#include "Particles.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// Measure the amount of ms that takes to execute:
// App constructor, Awake, Start and CleanUp
// LOG the result

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(ptimer);

	win = new Window(true);
	input = new Input(true);
	render = new Render(true);
	tex = new Textures(true);
	audio = new Audio(true);
	collisions = new Collisions(false);
	scene = new Scene(true);
	map = new Map(false);
	fade = new FadeToBlack(true);
	logoScreen = new LogoScreen(false);
	titleScreen = new TitleScreen(false);
	endingScreen = new EndingScreen(false);
	pathfinding = new PathFinding(false);
	entityman = new EntityManager(false);
	inputHandler = new InputHandler(true);
	particles = new Particles(true);

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(inputHandler);
	AddModule(tex);
	AddModule(audio);
	AddModule(collisions);
	AddModule(logoScreen);
	AddModule(titleScreen);
	AddModule(endingScreen);
	AddModule(pathfinding);
	AddModule(scene);
	AddModule(map);
	AddModule(entityman);
	AddModule(particles);

	AddModule(fade);

	// Render last to swap buffer
	AddModule(render);

	
	PERF_PEEK(ptimer);
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();
}

void App::AddModule(Module* module)
{
	//module->Enable();
	modules.Add(module);
}

// Called before render is available
bool App::Awake()
{
	PERF_START(ptimer);

	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	bool ret = false;

	// Load config from XML
	config = LoadConfig(configFile);

	if (config.empty() == false)
	{
		ret = true;
		configApp = config.child("app");

		// Read the title from the config file
		title.Create(configApp.child("title").child_value());
		organization.Create(configApp.child("organization").child_value());

        // Read from config file your framerate cap
		newMaxFramerate = configApp.attribute("framerate_cap").as_int();
	}

	if (ret == true)
	{
		ListItem<Module*>* item;
		item = modules.start;

		while ((item != NULL) && (ret == true))
		{
			// Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}
	
	PERF_PEEK(ptimer);

	return ret;
}

// Called before the first frame
bool App::Start()
{
	PERF_START(ptimer);
	
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->isEnabled() ? item->data->Start() : true;
		item = item->next;
	}
	
	PERF_PEEK(ptimer);

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true) ret = false;
	if(ret == true) ret = PreUpdate();
	if(ret == true) ret = DoUpdate();
	if(ret == true) ret = PostUpdate();

	if (framerateCap == false)
	{
		if (newMaxFramerate > 0) cappedMs = (1000.0f / (float)newMaxFramerate);
		else cappedMs = -1;
	}
	else
		cappedMs = (1000.0f / 30.0f);

	if (app->render->vsync) vsyncStr.Create("on");
	else vsyncStr.Create("off");

	FinishUpdate();

	return ret;
}

// Load config from XML file
// NOTE: Function has been redesigned to avoid storing additional variables on the class
pugi::xml_node App::LoadConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description());
	else ret = configFile.child("config");

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
    frameCount++;
    lastSecFrameCount++;

    // Calculate the dt: differential time since last frame
	dt = frameTime.ReadSec();
	frameTime.Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	// This is a good place to call Load / Save methods
	if (loadGameRequested == true) LoadGame();
	if (saveGameRequested == true) SaveGame();
    
    // Framerate calculations
	// Amount of frames since startup
	// Amount of time since game start (use a low resolution timer)
	// Average FPS for the whole game life
	// Amount of ms took the last update
	// Amount of frames during the last second
    
	float averageFps = 0.0f;
	float secondsSinceStartup = 0.0f;
	uint32 lastFrameMs = 0;
	uint32 framesOnLastSec = 0;

	secondsSinceStartup = startupTime.ReadSec();
	averageFps = (float)frameCount / startupTime.ReadSec();
	lastFrameMs = frameTime.Read();
	if (lastSecFrameTime.Read() > 1000)
	{
		lastSecFrameTime.Start();
		prevLastSecFrameCount = lastSecFrameCount;
		lastSecFrameCount = 0;
	}
	framesOnLastSec = prevLastSecFrameCount;

	static char title[256];
	/*sprintf_s(title, 256, "FPS: %d / Av.FPS: %.2f / Last-frame MS: %02u / Last sec frames: %i Last dt: %.3f Time since startup: %.3f Frame Count: %I64u ",
			  averageFps, lastFrameMs, framesOnLastSec, dt, secondsSinceStartup, frameCount);*/
	sprintf_s(title, 256, "FPS: %d / Av.FPS: %.2f / Last-frame MS: %02u / Vsync: %s",
		framesOnLastSec, averageFps, lastFrameMs, vsyncStr.GetString());

	app->win->SetTitle(title);

    // L08: TODO 2: Use SDL_Delay to make sure you get your capped framerate
	if (cappedMs > lastFrameMs && cappedMs != -1)
	{
		// L08: TODO 3: Measure accurately the amount of time SDL_Delay() actually waits compared to what was expected
		delayTimer.Start();
		SDL_Delay(cappedMs - lastFrameMs);
		LOG("We waited for %d milliseconds and got back in %.6f", cappedMs - lastFrameMs, delayTimer.ReadMs());
	}
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;

	ListItem<Module*>* item;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;
		ret = pModule->isEnabled() ? pModule->PreUpdate() : true;
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;
		ret = pModule->isEnabled() ? pModule->Update(dt) : true;
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;
		ret = pModule->isEnabled() ? pModule->PostUpdate() : true;
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	PERF_START(ptimer);

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	PERF_PEEK(ptimer);

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void App::LoadGameRequest()
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist
	loadGameRequested = true;
}

// ---------------------------------------
void App::SaveGameRequest() const
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist and... should we overwriten
	saveGameRequested = true;
}

// ---------------------------------------
// Create a method to actually load an xml file
// then call all the modules to load themselves
bool App::LoadGame()
{
	bool ret = false;

	loadGameRequested = false;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("savegame.xml");

	if (result == NULL)
	{
		LOG("Could not load map xml file savegame.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		bool ret = true;

		pugi::xml_node node = doc.child("save");

		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			ret = item->data->LoadState(node.child(item->data->name.GetString()));
			item = item->next;
		}

		pugi::xml_node nodeEntity = node.child("entitymanager");
		for (int i = 0; i < app->entityman->entities.Count(); i++)
		{
			SString entityName;
			entityName.Create("Entity%d", i + 1);
			pugi::xml_node currentEntity = nodeEntity.child(entityName.GetString());
			app->entityman->entities.At(i)->data->position.x = currentEntity.attribute("x").as_int();
			app->entityman->entities.At(i)->data->position.y = currentEntity.attribute("y").as_int();
		}
	}

	return ret;
}

// L02: TODO 7: Implement the xml save method for current state
bool App::SaveGame() const
{
	bool ret = true;

	saveGameRequested = false;

	pugi::xml_document doc;
	pugi::xml_node node = doc.append_child("save");
	pugi::xml_node newNode;

	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		newNode = node.append_child(item->data->name.GetString());
		ret = item->data->SaveState(newNode);
		item = item->next;
	}

	pugi::xml_node nodeEntity = node.child("entitymanager");
	for (int i = 0; i < app->entityman->entities.Count(); i++)
	{
		SString entityName;
		entityName.Create("Entity%d", i + 1);
		pugi::xml_node currentEntity = nodeEntity.append_child(entityName.GetString());
		currentEntity.append_attribute("x").set_value(app->entityman->entities.At(i)->data->position.x);
		currentEntity.append_attribute("y").set_value(app->entityman->entities.At(i)->data->position.y);
	}

	doc.save_file("savegame.xml");

	return ret;
}