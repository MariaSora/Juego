#ifndef __FLYINGENEMY_H__
#define __FLYINGENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

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
public:
	float speed = 0.2f;
	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;

	SDL_Texture* flyingEnemy = nullptr; 

	Animation* currentAnimation = nullptr;
	Animation flyAnim, deathAnim; 

	iPoint initialPos;
	int distance;
	bool dir = false;
	bool type = true;

};

#endif // __FLYINGENEMY_H__