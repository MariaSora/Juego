#include "SceneIntro.h"
#include "App.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "Window.h"
#include "Render.h"
#include "Textures.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "GameOver.h"
#include "Audio.h"
#include "Log.h"


SceneIntro::SceneIntro(bool startEnabled) : Module(startEnabled)
{
	name.Create("sceneintro");
}

SceneIntro::~SceneIntro()
{

}

// Load assets
bool SceneIntro::Start()
{
	introTexture = app->tex->Load("Assets/Textures/mainMenu.png");
	app->map->Disable();
	app->render->camera.x = 0;
	//app->audio->PlayMusic("Assets/Audio/Music/introMusica.ogg");

	app->win->GetWindowSize(windowW, windowH);

	//el parametro id sirve para ponerles como un número y si queremos diferenciarlos por ello (nota por si me hace falta)
	SDL_Rect btPos = { 270, 390, 120,35 };
	playButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "PLAY", btPos, this);
	SDL_Rect btPos2 = { 470, 390, 170,40 };
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
	app->render->DrawTexture(introTexture,0, 0);

	if (playButton->isPressed) {
		app->fade->PassScreens(this, (Module*)app->scene, 20);
	}

	if (continueButton->isPressed) {
		app->fade->PassScreens(this, (Module*)app->scene, 20);
		app->LoadRequest();
	/*	load = true; */
	}

	if (settingsButton->isPressed) {
		if (popUpSettings == nullptr) {
			popUpSettings = (GuiControlPopUp*)app->guiManager->CreateGuiControl(GuiControlType::POPUP, 1, "", { 0,0,0,0 }, this);
			SDL_Rect btPos6 = { 620, 190, 30,30 };
			crossSButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "X", btPos6, this);
			SDL_Rect btPos8 = { 547, 200, 30,30 };
			fullscreen = (GuiControlCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 1, "Fullscreen", btPos8, this);
			SDL_Rect btPos9 = { 547, 250, 30,30 };
			vsync = (GuiControlCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 1, "VSync     ", btPos9, this);
			SDL_Rect btPos10 = { 468, 325, 110,10 };
			music = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 1, "Music ", btPos10, this, 0, 128);
			SDL_Rect btPos11 = { 468, 370, 110,10 };
			fx = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 1, "FX     ", btPos11, this, 0 , 128);
		}
	}
	if (popUpSettings != nullptr) {
		if (fullscreen->isChecked) {
			LOG("FULLSCREEN");
			app->win->ToggleFullscreen(true);
		}
		else {
			LOG("NO FULLSCREEN");
			app->win->ToggleFullscreen(false);
		}

		if (vsync->isChecked) {
			LOG("VSYNC");
			app->render->ToggleVSync(true);
		}
		else {
			LOG("NO VSYNC");
			app->render->ToggleVSync(false);
		}
		if (music->isPressed) {
			LOG("Music modified");
			app->audio->SetVolume(music->currentValue, true);
		}
		//fx in intro should be disabled
		if (fx->isPressed) {
			LOG("Fx modified");
			app->audio->SetVolume(fx->currentValue, false);
		}
	}
	if (crossSButton != nullptr) {
		if (crossSButton->isPressed || playButton->isPressed) {
		    app->guiManager->RemoveGuiControl(popUpSettings);
			popUpSettings = nullptr;
			app->guiManager->RemoveGuiControl(crossSButton);
			crossSButton = nullptr;
			app->guiManager->RemoveGuiControl(fullscreen);
			fullscreen = nullptr;	
			app->guiManager->RemoveGuiControl(vsync);
			vsync = nullptr;
			app->guiManager->RemoveGuiControl(music);
			music = nullptr;
			app->guiManager->RemoveGuiControl(fx);
			fx = nullptr;
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


