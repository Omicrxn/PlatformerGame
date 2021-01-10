#include "GuiManager.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"

#include "Defs.h"
#include "Log.h"

GuiManager::GuiManager(Input* input, Render* render, AudioManager* audio) : Module()
{
	name.Create("guimanager");

	this->render = render;
	this->audio = audio;
	this->input = input;
}

// Destructor
GuiManager::~GuiManager()
{}

// Called before render is available
bool GuiManager::Awake()
{
	LOG("Loading GUI Manager");
	bool ret = true;

	return ret;
}

// Called before quitting
bool GuiManager::CleanUp()
{
	if (!active) return true;

	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, uint32 id, SDL_Rect bounds, const char* text)
{
	GuiControl* ret = nullptr;

	switch (type)
	{
		// Create the corresponding GuiControl type
		case GuiControlType::BUTTON: ret = new GuiButton(id, bounds, text);  break;
		default: break;
	}

	// Created controls are added to the list
	if (ret != nullptr) controls.Add(ret);

	return ret;
}

bool GuiManager::Update(float dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	UpdateAll(dt, doLogic);

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}

	return true;
}

bool GuiManager::UpdateAll(float dt, bool doLogic)
{
	ListItem<GuiControl*>* control = controls.start;
	while (control != nullptr)
	{
		if (doLogic) 
			control->data->Update(input, dt);

		control->data->Draw(render);

		//if (!control->data->active)
		//	DestroyGuiControl(control->data);

		control = control->next;
	}

	return true;
}

void GuiManager::DestroyGuiControl(GuiControl* control)
{
	ListItem<GuiControl*>* item = controls.At(controls.Find(control));
	//app->tex->Unload(coontrol->texture);
	//control->texture = nullptr;
	RELEASE(control);

	controls.Del(item);
}