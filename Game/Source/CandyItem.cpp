#include "CandyItem.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

CandyItem::CandyItem() : Entity(EntityType::HEALITEM)
{
	name.Create("candyItem");
}

CandyItem::~CandyItem() {}

bool CandyItem::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	level = parameters.attribute("level").as_int();

	return true;
}

bool CandyItem::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	
	pbody = app->physics->CreateCircle(position.x, position.y, 10, bodyType::STATIC);  
	pbody->listener = this; 
	pbody->ctype = ColliderType::CANDYITEM;

	return true;
}

bool CandyItem::Update(float dt)
{
	if (isPicked1) {
		pbody->body->SetActive(false);
		SDL_DestroyTexture(texture);
	}
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x + 5, position.y + 5);
	
	return true;
}

bool CandyItem::CleanUp()
{
	return true;
}

void CandyItem::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		if (pbody->body->IsActive()) {
			isPicked1 = true;
			app->scene->player->points++;
		}
		break;
	}
}
