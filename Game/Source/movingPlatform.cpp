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

	return true;
}

bool MovingPlatform::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateRectangle(position.x + 16, position.y + 16, 48, 16, bodyType::KINEMATIC);
	pbody->ctype = ColliderType::MOVING_PLATFORM;

	return true;
}

bool MovingPlatform::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

 //   //movimiento de las plataformas
	//while (crono == 0){
	//	position.x++;
	//	crono++;
	//	if (crono >= 100){
	//		position.x--;
	//		crono--;
	//	}
	//}

	/*if (position.x >= 1040 && ro == false) {
		position.x += 100 * dt;
		if (position.x == 1220) ro = true;
	}

	if (position.x >= 1220 && ro == true) {
		position.x -= 100 * dt;
		if (position.x <= 1040) ro = false;
	}*/

	app->render->DrawTexture(texture, position.x - 8, position.y + 8);

	return true;
}

bool MovingPlatform::CleanUp()
{
	return true;
}
