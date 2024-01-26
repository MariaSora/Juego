#ifndef __CANDYITEM_H__
#define __CANDYITEM_H__

#include "Entity.h"
#include "Point.h"

#include "SDL/include/SDL.h"

struct SDL_Texture;

class CandyItem : public Entity
{
public:

	CandyItem();
	virtual ~CandyItem();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	bool isPicked1 = false;

private:
	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;
};

#endif // __CANDYITEM_H__