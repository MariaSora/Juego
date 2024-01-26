#include "GameOver.h"
#include "App.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "Window.h"
#include "Render.h"
#include "Textures.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "Audio.h"
#include "Log.h"


GameOver::GameOver(bool startEnabled) : Module(startEnabled)
{

}

GameOver::~GameOver()
{

}

// Load assets
bool GameOver::Start()
{
	gameOverTexture = app->tex->Load("Assets/Textures/gameOver.png");

	app->win->GetWindowSize(windowW, windowH);

	return true;
}

bool GameOver::Update(float dt)
{
	app->render->DrawTexture(gameOverTexture, 2, 0);
	return true;
}

bool GameOver::PostUpdate()
{
	return true;
}

