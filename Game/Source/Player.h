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
	void PlayerMovement(float dt);
	void PlayerTeleports();
	void LivesManagement();

public:
	float speed = 0.2f;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	const char* texturePath2;
	SDL_Texture* texture2 = NULL;	
	const char* texturePath3;
	SDL_Texture* texture3 = NULL;
	PhysBody* pbody;

	bool saltando = false;
	bool die = false;
	bool climb = false; 
	bool damage = false; 
	bool touchingP = false;
	bool touchingS = false;
	int remainingJumpSteps;
	int progreso_salto = 0;
	int duracion_salto = 320;

	bool atk = false;
	SDL_Texture* player = nullptr; 

	Animation* currentAnimation = nullptr; 
	Animation* currentLifeAnimation = nullptr; 
	Animation* currentStateAnimation = nullptr; 
	Animation idleAnim, climbAnim, climbIdleAnim, jumpAnim, walkAnim, attackAnim, dieAnim, damagedAnim, life0, life1, life2, life3, life4, life5, finishAnim, notFinishAnim;


	bool isFacingRight; 
	uint points;

	b2Transform PositionUpdate;

private:
	//bool LoadJumpAnim(pugi::xml_node& node, jumpAnims* jump);
	//bool LoadAllJumpAnims(pugi::xml_node animNode);

	b2Transform Ipos;

	MovingPlatform* movingplatform;
	bool inmovplat = false;
	iPoint moving = { 0,0 };
	
	bool c = false; 
	bool anim = false; 
	transparentWall* wall;
	Portal* portal;
};

#endif // __PLAYER_H__