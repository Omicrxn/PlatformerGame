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
}

void InputHandler::HandleInput()
{
	commandList.Clear();
	//Player Input
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		keyW->state = CommandState::REPEAT;
		commandList.Add(keyW);
	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_UP) {
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
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		keyS->state = CommandState::REPEAT;
		commandList.Add(keyS);
	}

	//DEBUG KEYS
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		keyF5->state = CommandState::DOWN;
		commandList.Add(keyF5);
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
	return ret;
}