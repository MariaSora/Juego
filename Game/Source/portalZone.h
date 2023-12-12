#ifndef __PORTALZONE_H__
#define __PORTALZONE_H__

#include "Entity.h"
#include "Point.h"

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
	


private:
	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;
};

#endif // __PORTALZONE_H__