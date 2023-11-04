#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

#include "Physics.h"

struct SDL_Texture;

//struct jumpAnim {
//	uint x;
//	uint y;
//	uint width;
//	uint height;
//
//};
//
//struct jumpAnims
//{
//	SString name;
//	int x;
//	int y;
//	int width;
//	int height;
//	List<jumpAnims*> jump; 
//};

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
	bool muere = false;
	int remainingJumpSteps;
	int progreso_salto = 0;
	int duracion_salto = 320;


	SDL_Texture* player = nullptr; 

	Animation* currentAnimation = nullptr; 
	Animation idleAnim, climbAnim, climbdownAnim, jumpAnim, walkAnim;

	bool isFacingRight; 

private:
	//bool LoadJumpAnim(pugi::xml_node& node, jumpAnims* jump);
	//bool LoadAllJumpAnims(pugi::xml_node animNode);

	b2Transform Ipos;
};

#endif // __PLAYER_H__