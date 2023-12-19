#ifndef __WALKINGENEMY_H__
#define __WALKINGENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "DynArray.h"

struct SDL_Texture;

class WalkingEnemy : public Entity
{
public:

	WalkingEnemy();
	virtual ~WalkingEnemy();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void MoveToPlayer(iPoint& enemyPos, float speed, const DynArray<iPoint>* path);

	void Attack();

public:

	float speed = 0.2f;
	SDL_Texture* texture;
	const char* texturePath;
	SDL_Texture* texture2;
	const char* drawPath;
	PhysBody* pbody;

	bool isFacingRight;
	bool isFacingLeft; 
	bool saltando = false; 

	SDL_Texture* walkingEnemy = nullptr;

	Animation* currentAnimation = nullptr;
	Animation idleAnim, walkAnim, deathAnim, attackAnim, jumpAnim, damageAnim;
	
	iPoint initialPos;

	int counter = 0; 
	bool type;
	int distance;

private: 
	iPoint enemyPos;
	iPoint playerPos;
	const DynArray<iPoint>* path;

};

#endif // __WALKINGENEMY_H__