#ifndef __FINALSCENE_H__
#define __FINALSCENE_H__

#include "Module.h"
#include "Player.h"
#include "HealItem.h"
#include "CandyItem.h"
#include "Checkpoints.h"
#include "GuiControlButton.h"
#include "GuiControlPopUp.h"


struct SDL_Texture;

class FinalScene : public Module
{
public:

	FinalScene(bool startDisabled);

	// Destructor
	virtual ~FinalScene();

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

	Player* GetPlayer();

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

	void Enable();
	void Disable();

private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;

public:

	HealItem* healItem;
	Player* player;
	CandyItem* candyItem;

	Checkpoints* checkpoint;
	GuiControlButton* crossPButton;
	GuiControlPopUp* popUpPause;

	float speedUI = 0.5f;

	PhysBody* pbody;
};

#endif // __FINALSCENE_H__