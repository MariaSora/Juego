#include "FlyingEnemy.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

FlyingEnemy::FlyingEnemy() : Entity(EntityType::FLYINGENEMY)
{
	name.Create("FlyingEnemy");
}

FlyingEnemy::~FlyingEnemy() {}

bool FlyingEnemy::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	dir = parameters.attribute("direction").as_bool();
	distance = parameters.attribute("distance").as_int();
	type = parameters.attribute("type").as_bool();

	flyAnim.LoadAnimation("flyingEnemy", "flyAnim");
	deathAnim.LoadAnimation("flyingEnemy", "deathAnim");

	return true;
}

bool FlyingEnemy::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateRectangleSensor(position.x + 16, position.y + 16, 16, 16, bodyType::KINEMATIC);
	pbody->ctype = ColliderType::FLYINGENEMY;

	initialPos.x = position.x;
	initialPos.y = position.y;

	return true;
}

bool FlyingEnemy::Update(float dt)
{	
	/*pbody->body->SetGravityScale(0);
	pbody->body->GetFixtureList()[0].SetSensor(true);*/
	currentAnimation = &flyAnim; 
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  

	pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0);

	currentAnimation->Update();
	SDL_Rect rect = currentAnimation->GetCurrentFrame();


	//app->render->DrawTexture(texture, position.x + 108, position.y + 50, &rect);

	//pbody->body->ApplyForce(b2Vec2(0.0f, -app->physics->world->GetGravity().y * pbody->body->GetMass()), pbody->body->GetWorldCenter(), true);
	//position.x++;

	if (type)
	{	
		app->render->DrawTexture(texture, position.x, position.y, &rect);
		if (!dir)
		{
			position.x++;
			if (position.x >= initialPos.x + distance)
			{
				dir = true;
			}
		}
		else
		{
			position.x--;
			if (position.x <= initialPos.x - distance)
			{
			 dir = false;
			}
		}
	}

	if (!type)
	{
		app->render->DrawTexture(texture, position.x, position.y, &rect);
		if (!dir)
		{
			position.y++;
			if (position.y >= initialPos.y + distance)
			{
				dir = true;
			}
		}
		else
		{
			position.y--;
			if (position.y <= initialPos.y - distance)
			{
				dir = false;
			}
		}
	}

	return true;
}

bool FlyingEnemy::CleanUp()
{
	return true;
}

void FlyingEnemy::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::STAIRS:
		LOG("Collision STAIRS");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}