#ifndef __FLYINGENEMY_H__
#define __FLYINGENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "DynArray.h"

struct SDL_Texture;

class FlyingEnemy : public Entity
{
public:

	FlyingEnemy();
	virtual ~FlyingEnemy();

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

	SDL_Texture* flyingEnemy = nullptr; 

	Animation* currentAnimation = nullptr;
	Animation flyAnim, deathAnim; 

	iPoint initialPos;
	int distance;
	bool dir = false;
	bool type = true;
	int temp;

private: 
	iPoint enemyPos; 
	iPoint playerPos; 
	const DynArray<iPoint>* path; 

	bool die = false;
};

#endif // __FLYINGENEMY_H__