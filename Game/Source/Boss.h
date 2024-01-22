#ifndef __BOSS_H__
#define __BOSS_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "DynArray.h"

struct SDL_Texture;

class Boss : public Entity
{
public:

	Boss();
	virtual ~Boss();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void MoveToPlayer(iPoint& enemyPos, float speed, const DynArray<iPoint>* path);

	void Attack();

	void BossFunctionality();

public:

	float speed = 0.2f;
	SDL_Texture* texture;
	SDL_Texture* texture2;
	const char* texturePath;

	const char* drawPath2;
	const char* drawPath3;
	PhysBody* pbody;

	bool isFacingRight;
	bool isFacingLeft; 
	bool saltando = false; 

	SDL_Texture* boss = nullptr;

	Animation* currentAnimation = nullptr;
	Animation idleAnim, walkAnim, deathAnim, attackAnim, jumpAnim, damageAnim;
	
	iPoint initialPos;

	int counter = 0; 
	bool type;
	int distance;

private: 
	iPoint BossPos;
	iPoint playerPos;
	const DynArray<iPoint>* path;

};

#endif // __BOSS_H__