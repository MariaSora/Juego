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
	pbody2 = app->physics->CreateRectangle(position.x, position.y, 8, 12, bodyType::DYNAMIC);
	pbody->listener = this; 
	pbody2->listener = this; 
	pbody->ctype = ColliderType::PARTICLES;
	pbody2->ctype = ColliderType::PARTICLES;

	return true;
}

bool Particles::Update(float dt)
{
	///*pbody->body->SetGravityScale(0);

	if (app->attack) {
		currentAnimation = &shootAnim;
	
		if (!alive) {
			//pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(app->scene->flyingEnemy->position.x), PIXEL_TO_METERS(app->scene->flyingEnemy->position.y)), 0);
			if (app->scene->flyingEnemy->type) {
				pbody->body->GetFixtureList()[0].SetSensor(true);
				position.y = app->scene->flyingEnemy->position.y + 25;
				position.x = app->scene->flyingEnemy->position.x + 20;
			}
			if (!app->scene->flyingEnemy->type) {
				pbody2->body->GetFixtureList()[0].SetSensor(true);
				position2.y = app->scene->flyingEnemy->position2.y + 25;
				position2.x = app->scene->flyingEnemy->position2.x + 20;
			}
			
			alive = true;
		}
		else {
			if (app->scene->flyingEnemy->type) {
				pbody->body->GetFixtureList()[0].SetSensor(false);
				position.y++;
				pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0);
			}
			if (!app->scene->flyingEnemy->type) {
				pbody2->body->GetFixtureList()[0].SetSensor(false);
				position2.y++;
				pbody2->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position2.x), PIXEL_TO_METERS(position2.y)), 0);
			}
		}
		if (app->scene->flyingEnemy->type) {
			currentAnimation->Update();
			SDL_Rect rect = currentAnimation->GetCurrentFrame();
			app->render->DrawTexture(texture, position.x - 5, position.y - 10, &rect);
		}
		if (!app->scene->flyingEnemy->type) {
			currentAnimation->Update();
			SDL_Rect rect = currentAnimation->GetCurrentFrame();
			app->render->DrawTexture(texture, position2.x - 5, position2.y - 10, &rect);
		}
		
	}
	
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
		if (app->godmode == false) {
			//app->vida--;
			app->scene->player->damage = true;
		}
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