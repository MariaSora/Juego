#ifndef __PORTALZONE_H__
#define __PORTALZONE_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"

#include "SDL/include/SDL.h"

struct SDL_Texture;

class portalZone : public Entity
{
public:

	portalZone();
	virtual ~portalZone();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:

	bool touchingW = false;
	bool touchingPortal = false;

	Animation* currentAnimation = nullptr;
	Animation turn;

private:
	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;

	bool type = true;
};

#endif // __PORTALZONE_H__