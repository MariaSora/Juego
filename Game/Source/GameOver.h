#ifndef __GAME_OVER_H__
#define __GAME_OVER_H__

#include "Module.h"
#include "GuiControl.h"
#include "GuiControlButton.h"


struct SDL_Texture;

class GameOver : public Module
{
public:
	//Constructor
	GameOver(bool startEnabled);

	//Destructor
	~GameOver();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt);

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate();

public:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* gameOverTexture = nullptr;
	GuiControlButton* restartButton;

private:

	uint windowW, windowH;
};

#endif

