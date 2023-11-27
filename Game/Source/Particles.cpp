#include "Particles.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Particles::Particles() : Entity(EntityType::PARTICLES)
{
	name.Create("Particles");
}

Particles::~Particles() {}

bool Particles::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	shootAnim.LoadAnimation("particles", "shootAnim");

	return true;
}

bool Particles::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateRectangleSensor(position.x + 16, position.y + 16, 16, 16, bodyType::KINEMATIC);
	pbody->ctype = ColliderType::PARTICLES;

	return true;
}

bool Particles::Update(float dt)
{
	/*pbody->body->SetGravityScale(0);
	pbody->body->GetFixtureList()[0].SetSensor(true);*/
	currentAnimation = &shootAnim; 
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	currentAnimation->Update();
	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	app->render->DrawTexture(texture, position.x + 8, position.y + 50, &rect);
	//app->render->DrawTexture(texture, position.x + 58, position.y + 50, &rect);
	//app->render->DrawTexture(texture, position.x + 108, position.y + 50, &rect);

	return true;
}

bool Particles::CleanUp()
{
	return true;
}

void Particles::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		break;
	}
}