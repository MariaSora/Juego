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
	SDL_Texture* texture4;
	const char* drawPath4;
	PhysBody* pbody;
	PhysBody* pbody2;

	SDL_Texture* flyingEnemy; 

	Animation* currentAnimation = nullptr;
	Animation flyAnim, deathAnim; 

	int distance;
	bool dir = false;
	bool type;
	int temp;

	//audios Fx
	int enemyShotFx;

private: 
	iPoint enemyPos; 
	iPoint playerPos; 
	const DynArray<iPoint>* path; 

};

#endif // __FLYINGENEMY_H__