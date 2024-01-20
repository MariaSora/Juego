#include "GuiControlPopUp.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"

GuiControlPopUp::GuiControlPopUp(uint32 id) : GuiControl(GuiControlType::POPUP, id)
{
	canClick = true;
	drawBasic = false;
}

GuiControlPopUp::~GuiControlPopUp()
{

}

bool GuiControlPopUp::Update(float dt)
{
	LOG("Draw PopUp");
	app->render->DrawRectangle({ 80, 50, 300, 200 }, 255, 134, 27, 210);
	
	return true; 
}

