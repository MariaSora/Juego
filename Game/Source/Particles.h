#ifndef __PARTICLES_H__
#define __PARTICLES_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Particles : public Entity
{
public:

	Particles();
	virtual ~Particles();

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
	PhysBody* pbody2;

	SDL_Texture* flyingEnemy = nullptr; 

	Animation* currentAnimation = nullptr;
	Animation shootAnim; 

	bool alive = true; 

};

#endif // __PARTICLES_H__