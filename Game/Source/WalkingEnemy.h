#ifndef __WALKINGENEMY_H__
#define __WLAKINGENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

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

public:

	bool isPicked = false;

	Animation* currentAnimation = nullptr;
	Animation idleAnim, walkAnim, deathAnim, attackAnim, jumpAnim;

private:

	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;
};

#endif // __WALKINGENEMY_H__