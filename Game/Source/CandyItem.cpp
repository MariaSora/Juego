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

CandyItem::CandyItem() : Entity(EntityType::CANDYITEM)
{
	name.Create("candyItem");
}

CandyItem::~CandyItem() {}

bool CandyItem::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool CandyItem::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateCircle(position.x, position.y, 10, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::CANDYITEM;

	return true;
}

bool CandyItem::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x + 5, position.y + 5);

	if (isPicked1) {
		pbody->body->SetActive(false);
		SDL_DestroyTexture(texture);
	}

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
		if (!app->godmode) {
			isPicked1 = true;
			app->scene->player->score++;
		}
		break;
	}
}