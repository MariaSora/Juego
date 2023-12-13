#ifndef __PORTAL_H__
#define __PORTAL_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"

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

	Animation turn;

private:
	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;

	b2Transform Ipos;


};

#endif // __PORTAL_H__