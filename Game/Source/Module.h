#ifndef __MODULE_H__
#define __MODULE_H__

#include "SString.h"

#include "PugiXml/src/pugixml.hpp"

class App;
class GuiControl; 

class Module
{
public:

	Module(bool startEnabled) : isEnabled(startEnabled)
	{}

	bool Init()
	{
		return true;
	}

	// Called before render is available
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

	virtual bool LoadState(pugi::xml_node node) {
		return true;
	}

	virtual bool SaveState(pugi::xml_node node) {
		return true;
	}

	virtual bool OnGuiMouseClickEvent(GuiControl* control)
	{
		return true;
	}

	virtual void Module::Enable()
	{
		if (!isEnabled)
		{
			isEnabled = true;
			Start();
		}
	}

	virtual void Module::Disable()
	{
		// TODO 0: Call CleanUp() for disabling a module
		if (isEnabled)
		{
			isEnabled = false;
			CleanUp();
		}
	}

public:

	SString name;
	bool isEnabled = true; 

};

#endif // __MODULE_H__