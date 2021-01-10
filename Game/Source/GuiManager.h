#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Module.h"
#include "GuiControl.h"

#include "List.h"

class Input;
class Render;
class AudioManager;

class GuiManager : public Module
{
public:

	GuiManager(Input* input, Render* render, AudioManager* audio);

	// Destructor
	virtual ~GuiManager();

	// Called before render is available
	bool Awake();

	bool Update(float dt);

	bool UpdateAll(float dt, bool doLogic);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	GuiControl* CreateGuiControl(GuiControlType type, uint32 id, SDL_Rect bounds, const char* text);
	void DestroyGuiControl(GuiControl* control);

	void AddGuiControl(GuiControl* control);

public:

	List<GuiControl*> controls;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;

	bool doLogic = false;

private:

	Input* input;
	Render* render;
	AudioManager* audio;
};

#endif // __GUIMANAGER_H__
