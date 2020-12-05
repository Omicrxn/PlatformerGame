#pragma once
#include "Module.h"
#include "List.h"

struct Command;
class InputHandler : public Module
{
public:
	InputHandler(bool startEnabled);
	void HandleInput();
	bool Update(float dt);
	// Methods to bind commands...
	void BindCommands();
	bool CleanUp();
public:
	List<Command*> commandList;

private:
	//PLAYER MOVEMENT
	Command* keyW;
	Command* keyS;
	Command* keyA;
	Command* keyD;

	//DEBUG KEYS
	Command* keyF1;
	Command* keyF3;
	Command* keyF5;
	Command* keyF6;
	Command* keyF9;
	Command* keyF10;
};