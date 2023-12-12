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
#include "FlyingEnemy.h"

Particles::Particles() : Entity(EntityType::PARTICLES)
{
	name.Create("Particles");
}

Particles::~Particles() {}

bool Particles::Awake() {

	position.x = parameters .attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	shootAnim.LoadAnimation("particles", "shootAnim");

	return true;
}

bool Particles::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateRectangle(position.x, position.y, 8, 12, bodyType::DYNAMIC);
	pbody->listener = this; 
	pbody->ctype = ColliderType::PARTICLES;

	return true;
}

bool Particles::Update(float dt)
{
	///*pbody->body->SetGravityScale(0);
	//pbody->body->GetFixtureList()[0].SetSensor(true);*/
	currentAnimation = &shootAnim; 
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  


	if (!alive) {	
		//pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(app->scene->flyingEnemy->position.x), PIXEL_TO_METERS(app->scene->flyingEnemy->position.y)), 0);
		position.y = app->scene->flyingEnemy->position.y + 20;
		position.x = app->scene->flyingEnemy->position.x + 10;
		alive = true;
	}
	else position.y++; pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0);


	currentAnimation->Update();
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x - 5, position.y - 10, &rect);

	////app->render->DrawTexture(texture, position.x + 58, position.y + 50, &rect);
	////app->render->DrawTexture(texture, position.x + 108, position.y + 50, &rect);

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
		alive = false; 
		app->vida--;
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		alive = false;
		break; 
	case ColliderType::WALKINGENEMY:
		LOG("Collision WALKINGENEMY");
		alive = false;
		break;
	}
}