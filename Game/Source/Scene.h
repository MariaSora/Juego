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

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

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

	Player* GetPlayer();

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	

public:
	Particles* particles;
	FlyingEnemy* flyingEnemy;
	HealItem* healItem;
	Player* player;
	CandyItem* candyItem;
	WalkingEnemy* walkingEnemy;
	Checkpoints* checkpoint;


};

#endif // __SCENE_H__