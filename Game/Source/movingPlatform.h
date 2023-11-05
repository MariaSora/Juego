#ifndef __MovingPlatform_H__
#define __MovingPlatform_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class MovingPlatform : public Entity
{
public:

	MovingPlatform();
	virtual ~MovingPlatform();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

private:

	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;
	int crono = 0; 
	bool ro = false;

};

#endif // __MovingPlatform_H__