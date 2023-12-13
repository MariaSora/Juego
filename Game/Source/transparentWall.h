#ifndef __TRANSPARENTWALL_H__
#define __TRANSPARENTWALL_H__

#include "Entity.h"
#include "Point.h"

#include "SDL/include/SDL.h"

struct SDL_Texture;

class transparentWall : public Entity
{
public:

	transparentWall();
	virtual ~transparentWall();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:

	bool touchingW = false;

private:
	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;

};

#endif // __TRANSPARENTWALL_H__