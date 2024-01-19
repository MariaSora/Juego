#include "transparentWall.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

transparentWall::transparentWall() : Entity(EntityType::WALL)
{
	name.Create("transparentWall");
}

transparentWall::~transparentWall() {}

bool transparentWall::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool transparentWall::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateRectangleSensor(position.x + 16, position.y + 16, 150, 32, bodyType::KINEMATIC); 
	pbody->listener = this;
	pbody->ctype = ColliderType::WALL;

	return true;
}

bool transparentWall::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x, position.y);


	if (touchingW)
	{
		SDL_SetTextureAlphaMod(texture, 100);
	}
	else
	{
		SDL_SetTextureAlphaMod(texture, 255);
	}

	return true;
}

bool transparentWall::CleanUp()
{
	return true;
}
