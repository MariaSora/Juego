#ifndef __HEALITEM_H__
#define __HEALITEM_H__

#include "Entity.h"
#include "Point.h"

#include "SDL/include/SDL.h"

struct SDL_Texture;

class HealItem : public Entity
{
public:

	HealItem();
	virtual ~HealItem();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:
	
	bool isPicked = false;

private:
	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;
	int level; 
};

#endif // __HEALITEM_H__