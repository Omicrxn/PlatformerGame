#pragma once
#include "Player.h"
#include "App.h"
#include "Scene.h"
#include "Pathfinding.h"

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
	virtual void Execute(Player* player = nullptr) {}
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
		}
		else if(state == CommandState::UP)
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
	void Execute(Player* player = nullptr)
	{
		app->SaveGameRequest();
	}
};

class LoadCommand : public Command
{
public:
	void Execute(Player* player = nullptr)
	{
		app->LoadGameRequest();
	}
};

class ViewLogicCommand : public Command
{
public:
	void Execute(Player* player = nullptr)
	{
		app->scene->DrawDebug();
		app->pathfinding->debug = !app->pathfinding->debug;
		app->collisions->debug = !app->collisions->debug;
	}
};

class VolumeUpCommand : public Command
{
public:
	void Execute(Player* player = nullptr)
	{
		app->scene->VolumeUp();
	}
};

class VolumeDownCommand : public Command
{
public:
	void Execute(Player* player = nullptr)
	{
		app->scene->VolumeDown();
	}
};

class GodMode : public Command
{
public:
	void Execute(Player* player = nullptr)
	{
		app->scene->player->godMode = !app->scene->player->godMode;
	}
};

class FPSCapTo30 : public Command
{
public:
	void Execute(Player* player = nullptr)
	{
		app->framerateCap = !app->framerateCap;
	}
};