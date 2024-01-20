#include "SceneIntro.h"
#include "App.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "Window.h"
#include "Textures.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "Audio.h"


SceneIntro::SceneIntro(bool startEnabled) : Module(startEnabled)
{

}

SceneIntro::~SceneIntro()
{

}

// Load assets
bool SceneIntro::Start()
{
	introTexture = app->tex->Load("Assets/Textures/mainMenu.png");
	buttonText = app->tex->Load("Assets/Textures/buttonText.png");
	//app->audio->PlayMusic("Assets/Audio/Music/introMusica.ogg");

	app->win->GetWindowSize(windowW, windowH);

	SDL_Rect btPos = { 270, 370, 120,35 };
	playButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "PLAY", btPos, this);
	SDL_Rect btPos2 = { 470, 370, 170,40 };
	continueButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "CONTINUE", btPos2, this);
	SDL_Rect btPos3 = {770, 10, 120,40 };
	settingsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "SETTINGS", btPos3, this);
	SDL_Rect btPos4 = {10, 10, 120,40 };
	creditsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "CREDITS", btPos4, this);
	SDL_Rect btPos5 = { 400, 550, 90,30 };
	exitButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "EXIT", btPos5, this);

	return true;
}

bool SceneIntro::Update(float dt)
{
	app->render->DrawTexture(introTexture, 0, 0);
	//app->render->DrawTexture(buttonText, 450, 350);
	//app->render->DrawTexture(buttonText, 250, 350);

	if (playButton->isPressed) {
		app->fade->PassScreens(this, (Module*)app->scene, 20);
	}

	if (settingsButton->isPressed) {
		if (popUpSettings == nullptr) {
			popUpSettings = (GuiControlPopUp*)app->guiManager->CreateGuiControl(GuiControlType::POPUP, 1, "", { 0,0,0,0 }, this);
			SDL_Rect btPos6 = { 720, 110, 30,30 };
			crossSButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "X", btPos6, this);
			//app->fonts->drawText("FullScreen", { 0,0,0,255 }, 420, 200, app->fonts->font3);
			SDL_Rect btPos8 = { 520, 200, 30,30 };
			fullscreen = (GuiControlCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 1, "", btPos8, this);		
		
			if (fullscreen->isPressed) app->win->fullscreen = true;
		}
	}
	if (crossSButton != nullptr) {
		if (crossSButton->isPressed) {
		    app->guiManager->RemoveGuiControl(popUpSettings);
			popUpSettings = nullptr;
			app->guiManager->RemoveGuiControl(crossSButton);
			crossSButton = nullptr;
			app->guiManager->RemoveGuiControl(fullscreen);
			fullscreen = nullptr;
		}
	}

	if (exitButton->isPressed) {
		SDL_DestroyWindow(app->win->window);
		SDL_Quit();
	}
	
	if (creditsButton->isFocused) {
		app->render->DrawRectangle({ 10, 22, 140, 72 }, 255, 255, 255, 190);
		app->fonts->drawText("CARLA  PUGA", { 255,0,175,255 }, 15, 30, app->fonts->font3);
		app->fonts->drawText("REBECA  FERNANDEZ", { 255,0,175,255 }, 15, 50, app->fonts->font3);
		app->fonts->drawText("MARIA  PERARNAU", { 255,0,175,255 }, 15, 70, app->fonts->font3);
	}


	return true;
}

bool SceneIntro::PostUpdate()
{ 
	return true;
}


