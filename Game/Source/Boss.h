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
	void LivesManagement(); 

public:

	float speed = 0.2f;
	SDL_Texture* texture;
	SDL_Texture* texture2;
	SDL_Texture* texture3;
	const char* texturePath;
	const char* texturePath2;

	const char* drawPath2;
	const char* drawPath3;
	PhysBody* pbody;

	bool isFacingRight;
	bool isFacingLeft; 
	bool saltando = false; 

	SDL_Texture* boss = nullptr;

	Animation* currentAnimation = nullptr;
	Animation* currentLifeAnimation = nullptr;
	Animation idleAnim, walkAnim, dieAnim, attackAnim, damagedAnim, life0, life1, life2, life3, life4, life5;
	
	iPoint initialPos;

	int counter = 0; 
	bool type;
	int distance;
	int vida = 5;

private: 
	iPoint BossPos;
	iPoint playerPos;
	const DynArray<iPoint>* path;

};

#endif // __BOSS_H__