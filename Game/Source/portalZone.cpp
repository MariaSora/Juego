#include "portalZone.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Animation.h"

portalZone::portalZone() : Entity(EntityType::WALL)
{
	name.Create("portalZone");
}

portalZone::~portalZone() {}

bool portalZone::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	type = parameters.attribute("type").as_bool();

	turn.LoadAnimation("portalZone", "turn");

	return true;
}

bool portalZone::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateRectangleSensor(position.x + 16, position.y + 16, 150, 32, bodyType::KINEMATIC); 
	pbody->ctype = ColliderType::WALL;

	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateRectangleSensor(position.x + 16, position.y + 16, 16, 16, bodyType::KINEMATIC);
	pbody->ctype = ColliderType::PORTAL;


	return true;
}

bool portalZone::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x, position.y);

	//currentAnimation->Update();


	if (touchingW && type)
	{
		SDL_SetTextureAlphaMod(texture, 100);
	}
	else
	{
		SDL_SetTextureAlphaMod(texture, 255);
	}

	if (!type)
	{
		//currentAnimation = &turn;
	}

	return true;
}

bool portalZone::CleanUp()
{
	return true;
}
