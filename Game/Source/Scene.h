#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "FlyingEnemy.h"
#include "WalkingEnemy.h"
#include "Particles.h"
#include "HealItem.h"
#include "CandyItem.h"
#include "Checkpoints.h"
#include "GuiControlButton.h"
#include "GuiControlPopUp.h"
#include "GuiControlCheckBox.h"
#include "GuiControlSlider.h"


struct SDL_Texture;

class Scene : public Module
{
public:

	Scene(bool startEnabled);

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//void Level2();

	Player* GetPlayer();

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

	bool gameover = false; 

	void Enable();
	void Disable();

private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;

public:
	Particles* particles;
	FlyingEnemy* flyingEnemy;
	HealItem* healItem;
	Player* player;
	CandyItem* candyItem;
	WalkingEnemy* walkingEnemy;
	Checkpoints* checkpoint;
	GuiControlButton* crossPButton = nullptr;
	GuiControlPopUp* popUpPause = nullptr;
	float speedUI = 0.5f;
	int num;

	GuiControlButton* settings = nullptr;
	GuiControlButton* exitB = nullptr;
	GuiControlButton* resume = nullptr;
	GuiControlPopUp* popUpSettings = nullptr;
	GuiControlButton* home = nullptr;

	GuiControlButton* settingsButton = nullptr;
	GuiControlButton* crossSButton = nullptr;
	GuiControlCheckBox* fullscreen = nullptr;
	GuiControlCheckBox* vsync = nullptr;
	GuiControlSlider* music = nullptr;
	GuiControlSlider* fx = nullptr;

	PhysBody* pbody;

	//Nivel2
	bool startLevel2 = false;
	bool open = false;
};

#endif // __SCENE_H__