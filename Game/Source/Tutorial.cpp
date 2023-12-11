#include "Tutorial.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Tutorial::Tutorial() : Entity(EntityType::TUTORIAL)
{
	name.Create("tutorial");
}

Tutorial::~Tutorial() {}

bool Tutorial::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();


	adAnim.LoadAnimation("tutorial", "adAnim"); 
	wsAnim.LoadAnimation("tutorial", "wsAnim");
	noAnim.LoadAnimation("tutorial", "noAnim");
	return true;
}

bool Tutorial::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateRectangle(position.x + 16, position.y + 16, 16, 16, bodyType::STATIC); 
	pbody->ctype = ColliderType::TUTORIAL;
	//pbody = app->physics->CreateRectangle(position.x + 16, position.y + 16, 16, 16, STATIC);
	//pbody->listener = this;
	//pbody->ctype = ColliderType::TUTORIAL;

	
	return true;
}

bool Tutorial::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	currentAnimation = &adAnim;

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
	
	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Tutorial::CleanUp()
{
	return true;
}

void Tutorial::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		//currentAnimation = &adAnim;
		break;
	}
}