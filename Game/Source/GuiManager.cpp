#include "GuiManager.h"
#include "App.h"
#include "Textures.h"

#include "GuiControlButton.h"
#include "GuiControlPopUp.h"
#include "GuiControlCheckBox.h"
#include "GuiControlSlider.h"
#include "Audio.h"

GuiManager::GuiManager(bool startEnabled) :Module(startEnabled)
{
	name.Create("guiManager");
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	return true;
}

// L15: DONE1: Implement CreateGuiControl function that instantiates a new GUI control and add it to the list of controls
GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, int minV, int maxV, SDL_Rect sliderBounds)
{
	GuiControl* guiControl = nullptr;

	//Call the constructor according to the GuiControlType
	switch (type)
	{
	case GuiControlType::BUTTON:
		guiControl = new GuiControlButton(id, bounds, text);
		break;
	case GuiControlType::POPUP:
		guiControl = new GuiControlPopUp(id);
		break;
	case GuiControlType::CHECKBOX:
		guiControl = new GuiControlCheckBox(id, text, bounds);
		break;
	case GuiControlType::SLIDER:
		guiControl = new GuiControlSlider(id, text, bounds, minV, maxV);
		break;
	}

	//Set the observer
	guiControl->observer = observer;

	// Created GuiControls are add it to the list of controls
	guiControlsList.Add(guiControl);

	return guiControl;
}

bool GuiManager::Update(float dt)
{	

	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		control->data->Update(dt);
		control = control->next;
	}

	return true;
}

bool GuiManager::CleanUp()
{
	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		RELEASE(control);
	}

	return true;

	return false;
}

bool GuiManager::RemoveGuiControl(GuiControl* controlToRemove)
{
	// Iterate through the list to find the control
	ListItem<GuiControl*>* currentControl = guiControlsList.start;
	ListItem<GuiControl*>* previousControl = nullptr;

	while (currentControl != nullptr && currentControl->data != controlToRemove)
	{
		previousControl = currentControl;
		currentControl = currentControl->next;
	}

	// If the control is found, remove it from the list
	if (currentControl != nullptr)
	{
		guiControlsList.Del(currentControl);
		return true;
	}

	// Control not found

	return false;
}


