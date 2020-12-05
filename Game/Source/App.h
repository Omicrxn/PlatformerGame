#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "PerfTimer.h"
#include "Timer.h"

#include "List.h"

#include "PugiXml/src/pugixml.hpp"

#define CONFIG_FILENAME		"config.xml"
#define SAVE_STATE_FILENAME "save_game.xml"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class Scene;
class Map;
class FadeToBlack;
class LogoScreen;
class TitleScreen;
class EndingScreen;
class Screen;
class PathFinding;
class EntityManager;
class InputHandler;


class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

    // L02: DONE 1: Create methods to request Load / Save
	void LoadGameRequest();
	void SaveGameRequest() const;

private:

	// Load config file
	// NOTE: It receives config document
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGame();
	bool SaveGame() const;

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	PathFinding* pathfinding;
	Scene* scene;
	Map* map;
	FadeToBlack* fade;
	LogoScreen* logoScreen;
	TitleScreen* titleScreen;
	EndingScreen* endingScreen;
	EntityManager* entityman;
	InputHandler* inputHandler;


private:

	int argc;
	char** args;
	SString title;
	SString organization;
	SString vsyncStr;

	bool framerateCap = true;

	List<Module *> modules;

	// Create new variables from pugui namespace
	// NOTE: Redesigned LoadConfig() to avoid storing this variables
	//pugi::xml_document configFile;
	//pugi::xml_node config;
	//pugi::xml_node configApp;

	mutable bool saveGameRequested;
	bool loadGameRequested;

	// Calculate some timing measures:
	PerfTimer ptimer;
	uint64 frameCount = 0;

	Timer startupTime;
	Timer frameTime;
	Timer lastSecFrameTime;
	
	uint32 lastSecFrameCount = 0;
	uint32 prevLastSecFrameCount = 0;
	
	float dt = 0.0f;
	
	uint32 newMaxFramerate;

	int	cappedMs = -1;
};

extern App* app;

#endif	// __APP_H__