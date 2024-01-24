#include "HealItem.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

HealItem::HealItem() : Entity(EntityType::HEALITEM)
{
	name.Create("healItem");
}

HealItem::~HealItem() {}

bool HealItem::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	level = parameters.attribute("level").as_int();

	return true;
}

bool HealItem::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	
	pbody = app->physics->CreateCircle(position.x, position.y, 10, bodyType::STATIC); 
	pbody->listener = this; 
	pbody->ctype = ColliderType::HEALITEM;

	return true;
}

bool HealItem::Update(float dt)
{
	if (isPicked) {
		pbody->body->SetActive(false);
		SDL_DestroyTexture(texture);
	}
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x + 5, position.y + 5);
	
	return true;
}

bool HealItem::CleanUp()
{
	return true;
}

void HealItem::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		isPicked = true; 
		break;
	}
}
