#include "Portal.h"
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
#include "transparentWall.h"
#include "Player.h"

Portal::Portal() : Entity(EntityType::PORTAL)
{
	name.Create("Portal");
}

Portal::~Portal() {}

bool Portal::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	app->positionportal2.x  = parameters.attribute("x2").as_int(); 
	app->positionportal2.y = parameters.attribute("y2").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	turn.LoadAnimation("portal", "turn");

	return true;
}

bool Portal::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateRectangleSensor(position.x + 16, position.y + 16, 10, 25, bodyType::KINEMATIC); 
	pbody->ctype = ColliderType::PORTAL;

	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = app->entityManager->entities.start; item != NULL; item = item->next)
	{
		pEntity = item->data;
		if (pEntity->type == EntityType::WALL)
		{
			wall = (transparentWall*)pEntity;
		}
	}

	return true;
}

bool Portal::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	//app->render->DrawTexture(texture, position.x, position.y);
	
	if (wall != NULL)
	{
		if (wall->touchingW) {

			turn.Update();
			SDL_Rect rect = turn.GetCurrentFrame();
			app->render->DrawTexture(texture, position.x, position.y, &rect, 1, SDL_FLIP_NONE);
		}
	}
	
	return true;
}

bool Portal::CleanUp()
{
	return true;
}
