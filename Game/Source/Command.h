#pragma once
#include "Player.h"
#include "App.h"
enum class CommandState
{
	IDLE,
	UP,
	DOWN, 
	REPEAT
	
};
class Command
{
public:
	virtual void Execute() {}
	virtual void Execute(Player* player) {}
	CommandState state = CommandState::IDLE;
};

class JumpCommand : public Command
{
public:
	
	
	void Execute(Player* player = nullptr) 
	{ 
		if(state == CommandState::REPEAT)
		{
			player->Jump();
		}else if(state == CommandState::UP)
		{
			player->SmallJump();
			state = CommandState::IDLE;
		}
	}
};
class RunLeftCommand : public Command
{
public:


	void Execute(Player* player = nullptr) 
	{
		player->Run(true);
	}
};
class RunRightCommand : public Command
{
public:


	void Execute(Player* player = nullptr)
	{
		player->Run(false);
	}
};

class SaveCommand : public Command
{
public:
	void Execute()
	{
		app->SaveGameRequest();
	}
};
