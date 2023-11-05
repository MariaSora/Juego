#include "MovingPlatform.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

MovingPlatform::MovingPlatform() : Entity(EntityType::MOVINGPLATFORM)
{
	name.Create("movingPlatform");
}

MovingPlatform::~MovingPlatform() {}

bool MovingPlatform::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	ro = parameters.attribute("direction").as_bool();
	distance = parameters.attribute("distance").as_int();
	type = parameters.attribute("type").as_bool();

	return true;
}

bool MovingPlatform::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateRectangle(position.x + 16, position.y + 16, 48, 16, bodyType::KINEMATIC);
	pbody->ctype = ColliderType::MOVING_PLATFORM;
	initialPos.x = position.x;
	initialPos.y = position.y;

	return true;
}

bool MovingPlatform::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	/*position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;*/

	//movimiento de las plataformas

	if (type)
	{
		if (!ro)
		{
			position.x++;
			if (position.x >= initialPos.x + distance)
			{
				ro = true;
			}
		}
		else
		{
			position.x--;
			if (position.x <= initialPos.x - distance)
			{
				ro = false;
			}
		}
	}
	else
	{
		if (!ro)
		{
			position.y++;
			if (position.y >= initialPos.y + distance)
			{
				ro = true;
			}
		}
		else
		{
			position.y--;
			if (position.y <= initialPos.y - distance)
			{
				ro = false;
			}
		}
	}


	pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x + 0.3), PIXEL_TO_METERS(position.y + 0.35)), 0);

	app->render->DrawTexture(texture, position.x - 8, position.y + 8);

	return true;
}

bool MovingPlatform::CleanUp()
{
	return true;
}
