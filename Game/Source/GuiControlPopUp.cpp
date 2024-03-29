#include "GuiControlPopUp.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Scene.h"
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
	app->render->DrawRectangle({ int((-app->render->camera.x * app->scene->speedUI) + 130), 90, 200, 110 }, 255, 134, 27, 240);
	
	return true; 
}

