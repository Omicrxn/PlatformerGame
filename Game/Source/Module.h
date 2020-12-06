#ifndef __MODULE_H__
#define __MODULE_H__

#include "SString.h"

#include "PugiXml/src/pugixml.hpp"

struct Collider;

class Module
{
public:

	Module(bool startActive) : active(startActive)
	{}

	void Module::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Module::Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	bool isEnabled() { return active; }

	// Called before render is available
	// L01: DONE 5: Sending config file to all modules
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

    // L02: DONE 2: Create new virtual methods to Load / Save state
	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&) const
	{
		return true;
	}

	//Called when two colliders are intersecting
	//and the module is registered as the listener
	virtual void OnCollision(Collider* c1, Collider* c2) {}

public:

	SString name;
	bool active;
};

#endif // __MODULE_H__