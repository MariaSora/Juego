#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "movingPlatform.h"
#include "transparentWall.h"
#include "Portal.h"
#include "SDL/include/SDL.h"

#include "Physics.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void OnExitCollision(PhysBody* physA, PhysBody* physB);

public:
	float speed = 0.2f;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;

	//audios Fx
	int pickCoinFxId;
	int attackFx, jumpFx, killFx;

	bool saltando = false;
	bool die = false;
	bool climb = false; 
	bool damage = false; 
	bool touchingP = false;
	bool touchingS = false;
	int remainingJumpSteps;
	int progreso_salto = 0;
	int duracion_salto = 320;


	SDL_Texture* player = nullptr; 

	Animation* currentAnimation = nullptr; 
	Animation idleAnim, climbAnim, climbIdleAnim, jumpAnim, walkAnim, attackAnim, dieAnim, damagedAnim;

	bool isFacingRight; 

private:
	//bool LoadJumpAnim(pugi::xml_node& node, jumpAnims* jump);
	//bool LoadAllJumpAnims(pugi::xml_node animNode);

	b2Transform Ipos;

	MovingPlatform* movingplatform;
	bool inmovplat = false;
	int moving = 0;

	bool On = false;
	transparentWall* wall;
	Portal* portal;
};

#endif // __PLAYER_H__