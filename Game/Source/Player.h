#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"


struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:
	float speed = 0.2f;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;
	int pickCoinFxId;

	bool saltando = false;
	int progreso_salto = 0;
	int duracion_salto = 320;

	SDL_Texture* player = nullptr; 

	Animation* currentAnimation = nullptr; 
	Animation idleAnim, climbAnim, climbdownAnim, jumpLAnim, jumpRAnim, walkLAnim, walkRAnim;
};

#endif // __PLAYER_H__