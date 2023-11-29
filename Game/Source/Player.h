#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
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

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

public:
	float speed = 0.2f;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;

	//audios Fx
	int pickCoinFxId;
	int attackFx, jumpFx, killFx;

	bool saltando = false;
	bool muere = false;
	bool climb = false; 
	bool touchingP = false;
	bool touchingS = false;
	int remainingJumpSteps;
	int progreso_salto = 0;
	int duracion_salto = 320;


	SDL_Texture* player = nullptr; 

	Animation* currentAnimation = nullptr; 
	Animation idleAnim, climbAnim, climbIdleAnim, jumpAnim, walkAnim, attackAnim;

	bool isFacingRight; 

private:
	//bool LoadJumpAnim(pugi::xml_node& node, jumpAnims* jump);
	//bool LoadAllJumpAnims(pugi::xml_node animNode);

	b2Transform Ipos;
};

#endif // __PLAYER_H__