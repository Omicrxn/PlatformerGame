#include "App.h"
#include "Window.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "LogoScreen.h"
#include "TitleScreen.h"
#include "EndingScreen.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;

	win = new Window(true);
	input = new Input(true);
	render = new Render(true);
	tex = new Textures(true);
	audio = new Audio(true);
	scene = new Scene(false);
	map = new Map(false);
	fade = new FadeToBlack(true);
	logoScreen = new LogoScreen(true);
	titleScreen = new TitleScreen(false);
	endingScreen = new EndingScreen(false);
	player = new Player(false);


	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(tex);
	AddModule(audio);
	AddModule(logoScreen);
	AddModule(titleScreen);
	AddModule(endingScreen);
	AddModule(scene);
	AddModule(map);
	AddModule(player);

	AddModule(fade);

	// Render last to swap buffer
	AddModule(render);
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

	modules.clear();
}

void App::AddModule(Module* module)
{
	//module->Enable();
	modules.add(module);
}

// Called before render is available
bool App::Awake()
{
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

	return ret;
}

// Called before the first frame
bool App::Start()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->isEnabled() ? item->data->Start() : true;
		item = item->next;
	}

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
{}

// ---------------------------------------------
void App::FinishUpdate()
{
	// This is a good place to call Load / Save methods
	if (loadGameRequested == true) LoadGame();
	if (saveGameRequested == true) SaveGame();
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
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

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
	if (index < argc) return args[index];
	else return NULL;
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

	doc.save_file("savegame.xml");

	return ret;
}