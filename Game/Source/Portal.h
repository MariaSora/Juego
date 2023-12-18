#ifndef __PORTAL_H__
#define __PORTAL_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "transparentWall.h"

#include "SDL/include/SDL.h"

struct SDL_Texture;

class Portal : public Entity
{
public:

	Portal();
	virtual ~Portal();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:

	bool touchingPortal = false;

	Animation idlePortal, closePortal;
	SDL_Texture* texture;

private:

	const char* texturePath;
	PhysBody* pbody;
	transparentWall* wall;
};

#endif // __PORTAL_H__