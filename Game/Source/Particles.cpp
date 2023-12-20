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

	type = parameters.attribute("type").as_bool();
	if (type) {
		position.x = parameters.attribute("x").as_int();
		position.y = parameters.attribute("y").as_int();
		alive = parameters.attribute("alive").as_bool();
		texturePath = parameters.attribute("texturepath").as_string();
	}
	if (!type) {
		position2.x = parameters.attribute("x").as_int();
		position2.y = parameters.attribute("y").as_int();
		alive2 = parameters.attribute("alive").as_bool();
		texturePath2 = parameters.attribute("texturepath").as_string();
	}

	shootAnim.LoadAnimation("particles", "shootAnim");

	return true;
}

bool Particles::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	texture2 = app->tex->Load(texturePath);
	pbody = app->physics->CreateRectangle(position.x, position.y, 8, 12, bodyType::DYNAMIC);
	pbody2 = app->physics->CreateRectangle(position2.x, position2.y, 8, 12, bodyType::DYNAMIC);
	pbody->listener = this; 
	pbody2->listener = this; 
	pbody->ctype = ColliderType::PARTICLES;
	pbody2->ctype = ColliderType::PARTICLES;

	return true;
}

bool Particles::Update(float dt)
{

	currentAnimation = &shootAnim;
	if (app->attack && type) {
		if (!alive && app->scene->flyingEnemy != nullptr) { 
			pbody->body->GetFixtureList()[0].SetSensor(true);
			position.y = app->scene->flyingEnemy->position.y + 25;
			position.x = app->scene->flyingEnemy->position.x + 20;
		
			alive = true;
		}
		else {
			pbody->body->GetFixtureList()[0].SetSensor(false);
			position.y++;
			pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0);
		}
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x - 5, position.y - 10, &rect);
	}

	if (app->attack2 && !type) {
		if (!alive2 && app->scene->flyingEnemy != nullptr) {
			pbody2->body->GetFixtureList()[0].SetSensor(true);
			position2.y = app->scene->flyingEnemy->position2.y + 25;
			position2.x = app->scene->flyingEnemy->position2.x + 20;

			alive2 = true;
		}
		else {
				pbody2->body->GetFixtureList()[0].SetSensor(false);
				position2.y++;
				pbody2->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position2.x), PIXEL_TO_METERS(position2.y)), 0);
		}
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture2, position2.x - 5, position2.y - 10, &rect);
	}
	currentAnimation->Update();
	
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
		LOG("PARTICLES COLLIDE WITH PLAYER");
		alive = false; 
		alive2 = false; 
		if (app->godmode == false) {
			//app->vida--;
			app->scene->player->damage = true;
		}
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		alive = false;
		alive2 = false;
		break; 
	case ColliderType::WALKINGENEMY:
		LOG("Collision WALKINGENEMY");
		alive = false;
		alive2 = false;
		break;
	}
}