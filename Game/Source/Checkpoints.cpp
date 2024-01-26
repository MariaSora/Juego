#include "Checkpoints.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "Module.h"

Checkpoints::Checkpoints() : Entity(EntityType::CHECKPOINT)
{
	name.Create("checkpoint");
}

Checkpoints::~Checkpoints() {}

bool Checkpoints::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	num = parameters.attribute("num").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Checkpoints::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	
	pbody = app->physics->CreateRectangleSensor(position.x, position.y, 20, 100, bodyType::KINEMATIC);
	pbody->listener = this; 
	pbody->ctype = ColliderType::CHECKPOINT;

	return true;
}

bool Checkpoints::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	if (intouch) {

		switch (num) {
		case 1:
			LOG("Checkpoint 1");
			//Guardar la posicion del checkpoint en una variable nueva para que cuando el player muera se pueda actualizar la posicion en ese checkpoint
			app->scene->player->PositionUpdate.p.x = PIXEL_TO_METERS(position.x);
			app->scene->player->PositionUpdate.p.y = PIXEL_TO_METERS(position.y);
			break;
		case 2:
			LOG("Checkpoint 2");
			app->scene->player->PositionUpdate.p.x = PIXEL_TO_METERS(position.x); 
			app->scene->player->PositionUpdate.p.y = PIXEL_TO_METERS(position.y);
			break;
		case 3:
			LOG("Checkpoint 3");
			app->scene->startLevel1 = false;
			app->scene->startLevel2 = true;
			app->scene->Level2();
			break;
		}
	}
	intouch = false;

	app->render->DrawTexture(texture, position.x + 5, position.y + 5);
	
	return true;
}

bool Checkpoints::CleanUp()
{
	return true;
}

void Checkpoints::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		intouch = true;
		break;
	}
}
