#ifndef __SCENE_INTRO_H__
#define __SCENE_INTRO_H__

#include "Module.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "GuiControlCheckBox.h"
#include "GuiControlPopUp.h"

struct SDL_Texture;

class SceneIntro : public Module
{
public:
	//Constructor
	SceneIntro(bool startEnabled);

	//Destructor
	~SceneIntro();

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
	SDL_Texture* introTexture = nullptr;
	SDL_Texture* buttonText = nullptr;
	GuiControlButton* playButton;
	GuiControlButton* continueButton;
	GuiControlButton* settingsButton;
	GuiControlButton* creditsButton;
	GuiControlButton* exitButton;
	GuiControlButton* crossSButton;
	GuiControlPopUp* popUpSettings;
	GuiControlCheckBox* fullscreen;

private:

	uint windowW, windowH;
};

#endif

