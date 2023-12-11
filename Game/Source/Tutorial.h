#ifndef __TUTORIAL_H__
#define __TUTORIAL_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Tutorial : public Entity
{
public:

	Tutorial();
	virtual ~Tutorial();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);
public:
	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;

	bool isPicked = false;
	Animation* currentAnimation = nullptr;
	Animation adAnim, wsAnim, noAnim;

private:

};

#endif // __TUTORIAL_H__