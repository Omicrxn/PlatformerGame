#include "InputHandler.h"
#include "Command.h"
#include "App.h"
#include "Input.h"
#include "SDL/include/SDL_scancode.h"

InputHandler::InputHandler(bool startEnabled) : Module(startEnabled)
{
	BindCommands();
}

void InputHandler::BindCommands() 
{
	keyW = new JumpCommand();
	keyA = new RunLeftCommand();
	keyD = new RunRightCommand();
	keyF5 = new SaveCommand();
	keyF6 = new LoadCommand();
	keyF9 = new ViewLogicCommand();
	
	keyPlus = new VolumeUpCommand();
	keyMinus = new VolumeDownCommand();
}

void InputHandler::HandleInput()
{
	commandList.Clear();

	// Player Input
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		keyW->state = CommandState::REPEAT;
		commandList.Add(keyW);
	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_UP) 
	{
		keyW->state = CommandState::UP;
		commandList.Add(keyW);
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		keyA->state = CommandState::REPEAT;
		commandList.Add(keyA);
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		keyD->state = CommandState::REPEAT;
		commandList.Add(keyD);
	}

	// DEBUG Keys
	// F5 Save the current game state
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		keyF5->state = CommandState::DOWN;
		commandList.Add(keyF5);
	}
	// F6 Load the previous state (even across levels)
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		keyF6->state = CommandState::DOWN;
		commandList.Add(keyF6);
	}
	// F9 View colliders / logic / PATHS
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		keyF9->state = CommandState::DOWN;
		commandList.Add(keyF9);
	}

	// Change volume with +/- from the numeric keyboard
	if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
	{
		keyPlus->state = CommandState::DOWN;
		commandList.Add(keyPlus);
	}
	if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
	{
		keyMinus->state = CommandState::DOWN;
		commandList.Add(keyMinus);
	}
}

bool InputHandler::Update(float dt) 
{
	bool ret = true;

	HandleInput();

	return ret;
}

bool InputHandler::CleanUp()
{
	bool ret = true;

	delete keyW;
	delete keyA;
	delete keyD;
	delete keyF5;
	delete keyF6;
	delete keyF9;
	delete keyPlus;
	delete keyMinus;

	return ret;
}